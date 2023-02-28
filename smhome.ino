#include <Servo.h>

#include <EasyMFRC522.h>
#include <RfidDictionaryView.h>

#include <Adafruit_Fingerprint.h>
#include <Servo.h>

// Define ports.
SoftwareSerial mySerial(2, 3);
Servo Servo1;


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
bool lastState = -1;
int relay1 = 8;
int servoPin = 4;

void setup()  
{
    pinMode(relay1, OUTPUT);
    digitalWrite(relay1, HIGH);    
    Servo1.attach(servoPin); 

  Serial.begin(9600);
  while (!Serial); 
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");
  // set the data rate for the sensor serial port
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}

void loop()                     // run over and over again
{ 
  int id;
  id = getFingerprintIDez();

// By defaults, make sure that the servo always stay in 0 degree.
   Servo1.write(0); 
   delay(1000); 


  if( id == -1 ){
     if( lastState == 0){
         lastState = -1;
         delay(2000);
     }
  }

  else if( id != -1){
       digitalWrite(relay1, LOW);
       delay(2000);
       digitalWrite(relay1, HIGH);
  }
  delay(50);            // No need to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  // OK success!
  p = finger.image2Tz();
  switch (p) {


    case FINGERPRINT_OK:
      Serial.println("Image converted");   
// Rotate 90 degrees every time FINGERPRINT_OK
      Servo1.write(90); 
      delay(1000); 
      break;


    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   

  // found a match!

  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  Serial.println(p);
   if( p == 0 ){
      lastState = 0;
   }

  if (p != FINGERPRINT_OK)  return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  lastState = 1;

  // found a match!

  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
