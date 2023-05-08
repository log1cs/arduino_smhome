//
// (C) 2023, Pozitron/Lycoris Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <Servo.h>
#include <Adafruit_Fingerprint.h>
#include <Keypad.h>

/// Define SoftwareSerial ports.
SoftwareSerial mySerial(2, 3);

/// Servo
Servo ServoA;
int servoPin = 12;

// Define start angle.
int position = 0;

/// Fingerprint
Adafruit_Fingerprint fp = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;

//// Keypad - Password
const byte ROWS = 4; // 4 rows
const byte COLS = 4; // 4 columns

// Keylayout
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Connected pins
byte rowPins[ROWS] = {11, 10, 9, 8}; 
byte colPins[COLS] = {7, 6, 5, 4};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// Password
const String Password1 = "ABCD";
const String Password2 = "1234";
const String Password3 = "0001";
const String Password4 = "1000";
String password;

//// Main code
void setup() {
  // Attach servo1.
  ServoA.attach(servoPin);
  ServoA.write(0);
  /// Fingerprint sensor module setup.
  // Set the speed to 9600 baud.
  Serial.begin(9600);
  fp.begin(57600);

  Serial.println("Checking for Fingerprint sensor...");

  if (fp.verifyPassword()) {
    Serial.println("FP sensor detected");
  } else {
    Serial.println("FP sensor not detected. Check your connection.");
    while (1) {
      delay(1); 
    }
  }

  // Maximum password length is password.reserve(x) digits
  password.reserve(4);

  // Let's go!
  Serial.println("Welcome!");
  Serial.println("Please place your finger on the fingerprint sensor or enter the password to unlock.");
  Serial.println("Password: ");
}

void loop() {
  // Just stay in 0 will you?
  ServoA.write(0);

  // Continuously search for matched fingerprint
  fingerprintID = getFingerprintIDez();

/// To-do: Merging password unlock method with fingeprint one
/// (to clean up the code)

  /// Password
  char key = keypad.getKey();

  if (key) {
    Serial.print(key);

    if(key == '*') {
      Serial.println();
      Serial.println("You triggered the Reset button! Type password again from the start.");
      // Reset
      password = "";
    } else if(key == '#') {
      if(password == Password1 || password == Password2 || password == Password3 || password == Password4) {
        Serial.println();
        Serial.println("Credentials match! Unlocking.");
        ServoA.write(90);
        delay(4500);
        ServoA.write(0);
      } else {
        Serial.println();
        Serial.println("Wrong credentials. Try again");
      }

      // Reset
      password = "";
    } else {
      // Append new character to input password string
      password += key;
    }
  }

  /// Fingerprint
  if (fingerprintID == -1) {
      return 0;
  } else if (fingerprintID == 1 || fingerprintID == 2 || fingerprintID == 3 ||
	     fingerprintID == 4 || fingerprintID == 5 || fingerprintID == 6) {
        Serial.println();
        Serial.println("Credential match! Unlocking.");
        ServoA.write(90);
        delay(4500);
        ServoA.write(0);
  } else {
        Serial.println();
        Serial.println("Wrong credentials. Try again");
  }
  delay(50);
}

// getFingerprint function
int getFingerprintIDez() {
  uint8_t p = fp.getImage();

  if (p != FINGERPRINT_OK) {
      return -1;
  }

  p = fp.image2Tz();
  if (p != FINGERPRINT_OK) {
      return -1;
  }

  p = fp.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
      return -1;
  }

  // Return these values if fingerprint operation successful
  Serial.print("Found ID "); 
  Serial.print(fp.fingerID); 
  Serial.print(" with confidence of "); 
  Serial.println(fp.confidence);
  return fp.fingerID; 
}
