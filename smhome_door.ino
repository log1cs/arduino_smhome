#include <Servo.h>
#include <Adafruit_Fingerprint.h>
#include <Keypad.h>

// Define SoftwareSerial ports.
SoftwareSerial mySerial(2, 3);

// Servo
Servo ServoA;
int servoPin = 12;

// Fingerprint
Adafruit_Fingerprint fp = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;

//// Keypad and Password ////
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// Password
const String Password1 = "ABCD";
const String Password2 = "1234";
const String Password3 = "0001";
String password;

/// Main code
void setup() {
  // Attach servo1.
  ServoA.attach(servoPin);
  ServoA.write(0);

  // Fingerprint sensor module setup.
  Serial.begin(9600);
  fp.begin(57600);

  if (fp.verifyPassword()) {
    Serial.println("FP sensor detected");
  } else {
    Serial.println("FP sensor not detected!");
    while (1) {
       delay(1); 
    }
  }

  // Keypad
  password.reserve(4);

  // Let's go!
  Serial.println("Welcome!");
  Serial.println("Please touch the fingerprint sensor or enter the password to unlock the doors.");
}

void loop() {
  // Just stay in 0 will you?
  ServoA.write(0);

  // Continuously search for matched fingerprint
  fingerprintID = getFingerprintIDez();

  // Password
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if(key == '*') {
      // Reset
      password = "";
    } else if(key == '#') {
      if(password == Password1 || password == Password2 || password == Password3) {
        Serial.println("Credentials match! Unlocking the door.");
        ServoA.write(90);
        delay(5000);
        ServoA.write(0);
      } else {
        Serial.println("Wrong credentials. Try again");
      }

      // Reset
      password = "";
    } else {
      password += key; // append new character to input password string
    }
  }

  // Fingerprint
  if (fingerprintID == -1) {
      return 0;
  } else if (fingerprintID == 1 || fingerprintID == 2 || fingerprintID == 3 ||
	           fingerprintID == 4 || fingerprintID == 5 || fingerprintID == 6 ||
	           fingerprintID == 7 || fingerprintID == 8 || fingerprintID == 9 ||
	           fingerprintID == 10) {
       Serial.println("Credential match! Unlocking.");
       ServoA.write(90);                // Rotate 90 degrees
       delay(5000);
       ServoA.write(0);                 // Back to original state.
  }
  delay(50);

}

// returns -1 if failed, otherwise returns ID #
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
  Serial.print("Found ID #"); 
  Serial.print(fp.fingerID); 
  Serial.print(" with confidence of "); 
  Serial.println(fp.confidence);
  return fp.fingerID; 
}
