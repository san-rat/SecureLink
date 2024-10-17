# SecureLink: Automatic Door Locking System

## Project Overview
SecureLink is an automated door locking system designed to enhance security and streamline access for workplaces (meeting rooms, lecture halls). Using ESP32 and RFID technology, the system allows users to unlock doors via mobile application, RFID scan, or time-based triggers.

## Features
- **Smart Access Control**: Unlock doors using RFID or a mobile app.
- **Mobile Integration**: Remote control and monitor access with the Blynk app.
- **Increased Security**: Blocks unauthorized access, reducing break-in risks.
- **User-Friendly**: Designed with convenience for both residential and commercial applications.

## Components
To build this prototype, you’ll need the following components:
- **ESP32 microcontroller**: Manages the main logic and connectivity.
- **RFID Reader Module**: Used for scanning RFID tags to unlock the door.
- **Relay Modules**: A component which connects the microcontroller output and the lock system.
- **Solenoid Lock**: Controls the locking and unlocking mechanism.
- **Blynk Application**: Provides mobile app control over the system.
- **12V Power Supply**: Powers the solenoid lock.
- **Connecting Wires and Breadboard**: For prototyping and connections.

Optional:
- **Push Button**: For manual override or reset functionality.
- **LEDs**: To indicate system status (e.g., locked/unlocked).

## How It Works
SecureLink combines an ESP32 microcontroller, an RFID scanner, and a solenoid lock to control door access. The Blynk app allows users to interact with the system through their smartphones for convenient management. The system supports:
- **System Initialization**: Ensures all components are ready for use.
- **RFID Scan**: Unlocks the door when a registered tag is scanned.
- **Time-Based Unlock**: Allows door access based on pre-set schedules.
- **Manual Unlock**: Enables users to unlock the door directly from the app.

### RFID Card Authorization
To enhance security, the system allows only authorized RFID cards to unlock the door. To add or identify authorized cards:
1. Use the `RFID_Identification.ino` code to read the unique ID of each RFID card.
2. Run the identification code with your RFID reader connected to the ESP32. When a card is scanned, its unique ID will be displayed in hexadecimal format on the serial monitor.
3. Copy the hexadecimal ID and add it to the main code (`SecureLink.ino`) to authorize specific cards for door access.
4. Any card ID not in the authorized list will not trigger the unlocking mechanism.

This feature provides flexibility for easily managing which cards can access the system.

## Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/SecureLink.git
