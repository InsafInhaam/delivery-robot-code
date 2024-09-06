#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <HMC5883L.h>

// GPS Module
TinyGPSPlus gps;
SoftwareSerial ss(4, 3); // RX, TX for GPS module

HMC5883L compass; 

// Define Motor Pins
const int motorPin1 = 2;
const int motorPin2 = 3;
const int motorPin3 = 4;
const int motorPin4 = 5;
const int motorPin5 = 6; // Motor 3 control
const int motorPin6 = 7; // Motor 3 control
const int motorPin7 = 8; // Motor 4 control
const int motorPin8 = 9; // Motor 4 control
const int enableA = 10;  // PWM for Motor Driver 1
const int enableB = 11;  // PWM for Motor Driver 2

// Signal Lights
const int rightSignal = 12;
const int leftSignal = 13;
const int redLight = 14;

// Headlight
const int headlight = 15;
const int lightSensorPin = A0;

// Ultrasonic Sensor Pins
const int trigPinFront = 16;
const int echoPinFront = 17;
const int trigPinBack = 18;
const int echoPinBack = 19;
const int trigPinLeft = 20;
const int echoPinLeft = 21;
const int trigPinRight = 22;
const int echoPinRight = 23;

const float targetLat = 37.7749; // Target Latitude
const float targetLon = -122.4194; // Target Longitude

void setup() {
  Serial.begin(9600);
  ss.begin(9600);

  Wire.begin();
  compass = HMC5883L();
  compass.setScale(1.3);
  compass.setMeasurementMode(MEASUREMENT_CONTINUOUS);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(motorPin5, OUTPUT);
  pinMode(motorPin6, OUTPUT);
  pinMode(motorPin7, OUTPUT);
  pinMode(motorPin8, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);

  pinMode(rightSignal, OUTPUT);
  pinMode(leftSignal, OUTPUT);
  pinMode(redLight, OUTPUT);
  pinMode(headlight, OUTPUT);

  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinBack, OUTPUT);
  pinMode(echoPinBack, INPUT);
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);
}

void loop() {
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  if (gps.location.isUpdated()) {
    float currentLat = gps.location.lat();
    float currentLon = gps.location.lng();

    float distanceToTarget = gps.distanceBetween(currentLat, currentLon, targetLat, targetLon);
    float courseToTarget = gps.courseTo(currentLat, currentLon, targetLat, targetLon);

    // Read raw data from the compass
    MagnetometerRaw raw = compass.readRawAxis();
    float heading = atan2(raw.YAxis, raw.XAxis) * 180 / M_PI;
    if (heading < 0) {
      heading += 360;
    }

    float turnAngle = courseToTarget - heading;
    if (turnAngle > 180) {
      turnAngle -= 360;
    } else if (turnAngle < -180) {
      turnAngle += 360;
    }

    if (distanceToTarget > 5) { // Distance threshold in meters
      if (turnAngle < -10) {
        turnLeft();
      } else if (turnAngle > 10) {
        turnRight();
      } else {
        moveForward();
      }
    } else {
      stop();
    }
  }

  // Light Sensor for Headlight
  int lightSensorValue = analogRead(lightSensorPin);
  if (lightSensorValue < 300) {
    digitalWrite(headlight, HIGH);
  } else {
    digitalWrite(headlight, LOW);
  }

  delay(1000); // Update interval
}

void moveForward() {
  digitalWrite(redLight, LOW);
  analogWrite(enableA, 255);
  analogWrite(enableB, 255);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin5, HIGH);
  digitalWrite(motorPin6, LOW);
  digitalWrite(motorPin7, HIGH);
  digitalWrite(motorPin8, LOW);
}

void turnLeft() {
  digitalWrite(leftSignal, HIGH);
  digitalWrite(rightSignal, LOW);
  digitalWrite(redLight, LOW);
  analogWrite(enableA, 255);
  analogWrite(enableB, 255);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin5, LOW);
  digitalWrite(motorPin6, HIGH);
  digitalWrite(motorPin7, HIGH);
  digitalWrite(motorPin8, LOW);
  delay(500); // Adjust based on your robot's turning speed
  digitalWrite(leftSignal, LOW);
}

void turnRight() {
  digitalWrite(rightSignal, HIGH);
  digitalWrite(leftSignal, LOW);
  digitalWrite(redLight, LOW);
  analogWrite(enableA, 255);
  analogWrite(enableB, 255);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  digitalWrite(motorPin5, HIGH);
  digitalWrite(motorPin6, LOW);
  digitalWrite(motorPin7, LOW);
  digitalWrite(motorPin8, HIGH);
  delay(500); // Adjust based on your robot's turning speed
  digitalWrite(rightSignal, LOW);
}

void stop() {
  analogWrite(enableA, 0);
  analogWrite(enableB, 0);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin5, LOW);
  digitalWrite(motorPin6, LOW);
  digitalWrite(motorPin7, LOW);
  digitalWrite(motorPin8, LOW);
  digitalWrite(redLight, HIGH);
}
