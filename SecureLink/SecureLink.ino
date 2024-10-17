#define BLYNK_TEMPLATE_ID "TMPL6PbBAqUSA"
#define BLYNK_TEMPLATE_NAME "SecureLink App"
#define BLYNK_AUTH_TOKEN "UZlxhZO5vGQKziaO8rcxGa1vKL2gmcae"

#define BLYNK_PRINT Serial

#include <MFRC522.h>
#include <SPI.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <RTClib.h>

char auth[] = BLYNK_AUTH_TOKEN;
const char* ssid = "DNG";
const char* password = "87654321";

// RFID and relay pin setup
#define RST_PIN1 22
#define SDA_PIN1 21
#define RST_PIN2 15
#define SDA_PIN2 2

#define RELAY_PIN1 14
#define RELAY_PIN2 27

#define RED_PIN1 13
#define RED_PIN2 12

#define SDA_PIN 25
#define SCL_PIN 33

#define BUTTON_PIN1 34
#define BUTTON_PIN2 35

#define BUZZER_PIN 32

int timeInput1 = -1; // Variable to store time from Blynk V0 (in seconds)
int timeInput2 = -1; // Variable to store time from Blynk V1 (in seconds)

RTC_DS3231 rtc; // Create RTC instance

MFRC522 rfid1(SDA_PIN1, RST_PIN1);
MFRC522 rfid2(SDA_PIN2, RST_PIN2);

void doorUnlock(int relayPin, int RED_PIN, int buzzer_pin){
  
  digitalWrite(relay_pin, HIGH);
  digitalWrite(buzzer_pin, HIGH);
  digitalWrite(RED_PIN, LOW);
  delay(150);

  digitalWrite(relay_pin, HIGH);
  digitalWrite(buzzer_pin, LOW);
  digitalWrite(RED_PIN, LOW);
  delay(150);

  digitalWrite(relay_pin, HIGH);
  digitalWrite(buzzer_pin, HIGH);
  digitalWrite(RED_PIN, LOW);
  delay(150);

  digitalWrite(relay_pin, HIGH);
  digitalWrite(buzzer_pin, LOW);
  digitalWrite(RED_PIN, LOW);
  delay(1300);

  digitalWrite(relay_pin, LOW);
  digitalWrite(buzzer_pin, LOW);
  digitalWrite(RED_PIN, HIGH);
}

void checkAndUnlock(MFRC522 &rfid, int relayPin, int blynkVirtualPin, int RED_PIN, int BUZZER_PIN) {

    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        String UID = "";
        for (byte i = 0; i < rfid.uid.size; i++) {
            UID += String(rfid.uid.uidByte[i], HEX);
        }
        UID.toUpperCase(); // Ensure it's in uppercase

        if (UID == "D27C8C1B" || UID == "443AAFDE") { // Authorized UIDs
            Serial.println("Unlocking Relay Pin " + String(relayPin));

            if (UID == "D27C8C1B") {
                Blynk.virtualWrite(blynkVirtualPin, "User_1 has entered"); // Write to Blynk terminal
            } else {
                Blynk.virtualWrite(blynkVirtualPin, "User_2 has entered"); // Write to Blynk terminal
            }

            doorUnlock(relayPin, RED_PIN, BUZZER_PIN);

        } else { // If the UID is not authorized
            Serial.println("Unauthorized access attempt.");
            Blynk.virtualWrite(blynkVirtualPin, "Unauthorized Access"); // Write to Blynk terminal
        }

        rfid.PICC_HaltA(); // Halt the card
    }
}

void setup() {

  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);// Initialize I2C with custom SDA and SCL pins
  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1); // 
    // Stop if RTC is not found
  }

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" Connected to WiFi");

  // Connect to Blynk
  Blynk.begin(auth, ssid, password);

  // Initialize RFID and SPI
  SPI.begin();
  rfid1.PCD_Init();
  rfid2.PCD_Init();

  // Set relay pins as output
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);

  pinMode(RED_PIN1, OUTPUT);
  pinMode(RED_PIN2, OUTPUT);

  pinmode(BUZZER_PIN, OUTPUT);

  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);

  // Initially lock both relays
  digitalWrite(RELAY_PIN1, LOW);
  digitalWrite(RELAY_PIN2, LOW);

  digitalWrite(RED_PIN1, HIGH);
  digitalWrite(RED_PIN2, HIGH);

  digitalWrite(BUZZER_PIN, LOW);    
}

// Function to capture Time Input widget data from Virtual Pin 0 (V0)
BLYNK_WRITE(V0) {
  // Get the entire string data from the Time Input widget
  String timer1 = param.asStr();
  timeInput1 = atoi(timer1.c_str());

  // Display the full content of the Time Input widget
  Serial.print("Timer 1: ");
  Serial.println(timer1);
}

// Function to capture Time Input widget data from Virtual Pin 0 (V0)
BLYNK_WRITE(V1) {
  // Get the entire string data from the Time Input widget
  String timer2 = param.asStr();
  timeInput2 = atoi(timer2.c_str());

  // Display the full content of the Time Input widget
  Serial.print("Timer 2: ");
  Serial.println(timer2);
}

void loop() {
  Blynk.run(); // Run Blynk

  int buttonState1 = digitalRead(BUTTON_PIN1);
  int buttonState2 = digitalRead(BUTTON_PIN2);
  //Serial.println(buttonState1);

  if (buttonState1 == HIGH) { //MAKE THIS LOW
    Serial.println("Button1 is pressed");
    doorUnlock(RELAY_PIN1, RED_PIN1);
  }

  if (buttonState2 == HIGH) { //MAKE THIS LOW
    Serial.println("Button1 is pressed");
    doorUnlock(RELAY_PIN2, RED_PIN2);
  }

  // Check both RFID scanners and unlock the appropriate relay
  checkAndUnlock(rfid1, RELAY_PIN1, V2, RED_PIN1, BUZZER_PIN); // Pass Blynk virtual pin V2
  checkAndUnlock(rfid2, RELAY_PIN2, V2, RED_PIN2, BUZZER_PIN); // Pass Blynk virtual pin V2

  DateTime now = rtc.now(); // Get the current time from the RTC
  int seconds = now.hour()*3600 + now.minute()*60 + now.second();

  if (timeInput1 >= 0 && seconds == timeInput1) {
    // If the RTC time is greater than or equal to Time Input 1, print a message
    Serial.println("Timer 1 is triggered");
    doorUnlock(RELAY_PIN1, RED_PIN1, BUZZER_PIN);
  }

  if (timeInput2 >= 0 && seconds == timeInput2) {
    // If the RTC time is greater than or equal to Time Input 2, print a message
    Serial.println("Timer 2 is triggered");
    doorUnlock(RELAY_PIN2, RED_PIN2, BUZZER_PIN);
  }

  delay(500); // Delay for 1 second before checking again
}
