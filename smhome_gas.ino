#include<MQ2.h>
#include<Servo.h>

// Define MQ2 analog pin on the board. In this case we connected it in
// A0 - Analog 0. Might be difference depends on which pin you are
// connecting. 
#define MQ2pin 0

// Define the "smoke detected" value.
#define smoke 400

// SETTING UP SERVO
int servoPin = 8;
Servo ServoB;

float sensorValue;  

void setup() {
  // Attach servoB.
  ServoB.attach(servoPin);
  ServoB.write(0);

  // sets the serial port to 9600 baud
	Serial.begin(9600); 
	Serial.println("Starting");
  // allow the MQ2 to calibrate itself on bootup.
	delay(2000); 
}

void loop() {
  // Define & read analog values
	float sensorValue = analogRead(MQ2pin); 

  // Display the sensor values
	Serial.println("Sensor Value: ");
	Serial.print(sensorValue);
	
  // Kaboom!
  if(sensorValue > smoke)
  {
    Serial.print(" | Smoke");
    ServoB.write(90);
  } else {
    ServoB.write(0);
  }
  
	Serial.println("");

  // Read sensor value every 2s
	delay(2000); 
}
