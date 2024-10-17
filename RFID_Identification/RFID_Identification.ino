#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN     22  // Reset pin
#define SS_PIN      21  // SDA pin

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

const int relay_pin = 14;
const int beeper_pin = 12;

void setup() {
  Serial.begin(115200);
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  Serial.println("RFID reader initialized");

  pinMode(relay_pin, OUTPUT);
  pinMode(beeper_pin, OUTPUT);
}

void loop() {
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Show UID on serial monitor
    Serial.print("UID Tag :");
    String content = "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();
    if (content.substring(1) == "44 3A AF DE") { // Replace YOUR_CARD_UID with the UID of your card
      Serial.println("Authorized access");
      // Add your code for authorized access here
      digitalWrite(relay_pin, HIGH);
      digitalWrite(beeper_pin, HIGH);
      delay(150);                          //This is the code to allow the relay module to unlock the solonoid lock.
      
      digitalWrite(relay_pin, HIGH);
      digitalWrite(beeper_pin,LOW);
      delay(150);

      digitalWrite(relay_pin, HIGH);
      digitalWrite(beeper_pin, HIGH);
      delay(150);

      digitalWrite(relay_pin, HIGH);
      digitalWrite(beeper_pin,LOW);
      delay(1300);

      digitalWrite(relay_pin, LOW);
      digitalWrite(beeper_pin, LOW);

    } else {
      Serial.println("Access denied");

      digitalWrite(beeper_pin, HIGH);
      delay(750);

      digitalWrite(beeper_pin,LOW);
    }
  }
}
