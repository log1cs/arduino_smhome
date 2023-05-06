#include <Servo.h>
#include <Adafruit_Fingerprint.h>

// Define SoftwareSerial ports.
SoftwareSerial mySerial(2, 3);

// Servo
Servo ServoA;
int servoPin = 9;

// Fingerprint
Adafruit_Fingerprint fp = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;

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
}

void loop() {
  // Continuously search for matched fingerprint
  Serial.println("Searching for fingerprint...");
  fingerprintID = getFingerprintIDez();

  if (fingerprintID == -1) {
      Serial.println("Your credentials doesn't match! Search progress will be continue.");
      return 0;
  } else if (fingerprintID == 1 || fingerprintID == 2 || fingerprintID == 3 || fingerprintID == 4 || fingerprintID == 5) {
       Serial.println("Unlocked!");
       ServoA.write(90);                // Rotate 90 degrees
       delay(2000);
       ServoA.write(0);
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

  // Return these thing if FP sensor found a match.
  Serial.print("Found ID #"); 
  Serial.print(fp.fingerID); 
  Serial.print(" with confidence of "); 
  Serial.println(fp.confidence);
  return fp.fingerID; 
}
