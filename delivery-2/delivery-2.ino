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
const int enableA = 9;
const int enableB = 10;

// Signal Lights
const int rightSignal = 20;
const int leftSignal = 21;
const int redLight = 22;

// Headlight
const int headlight = 23;
const int lightSensorPin = A0;

// Ultrasonic Sensor Pins
const int trigPinFrontLeft = 6;
const int echoPinFrontLeft = 7;
const int trigPinFrontRight = 8;
const int echoPinFrontRight = 9;
// Add more pins as needed

const float targetLat = 37.7749; // Target Latitude
const float targetLon = -122.4194; // Target Longitude

void setup() {
  Serial.begin(9600);
  ss.begin(9600);

  Wire.begin();
  compass = HMC5883L();
  compass.setScale(1.3);
  compass.setMeasurementMode(HMC5883L_CONTINUOUS);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);

  pinMode(rightSignal, OUTPUT);
  pinMode(leftSignal, OUTPUT);
  pinMode(redLight, OUTPUT);
  pinMode(headlight, OUTPUT);

  pinMode(trigPinFrontLeft, OUTPUT);
  pinMode(echoPinFrontLeft, INPUT);
  pinMode(trigPinFrontRight, OUTPUT);
  pinMode(echoPinFrontRight, INPUT);
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

    // Define the Vector type if not included in the library
    struct Vector {
      float XAxis;
      float YAxis;
      float ZAxis;
    };

    Vector norm = compass.readNormalize();
    float heading = atan2(norm.YAxis, norm.XAxis);
    float headingDegrees = heading * 180/M_PI;
    if (headingDegrees < 0) {
      headingDegrees += 360;
    }

    float turnAngle = courseToTarget - headingDegrees;
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
  digitalWrite(redLight, HIGH);
}
