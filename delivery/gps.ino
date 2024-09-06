#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <HMC5883L.h>

TinyGPSPlus gps;
SoftwareSerial ss(4, 3); // RX, TX for GPS module

HMC5883L compass;

const float targetLat = 37.7749; // Target Latitude
const float targetLon = -122.4194; // Target Longitude

const int motorPin1 = 2;
const int motorPin2 = 3;
const int motorPin3 = 4;
const int motorPin4 = 5;
const int enableA = 9;
const int enableB = 10;

void setup() {
  Serial.begin(9600);
  ss.begin(9600);

  Wire.begin();
  compass = HMC5883L();
  compass.SetScale(1.3);
  compass.SetMeasurementMode(Measurement_Continuous);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);
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

    Vector norm = compass.ReadNormalized();
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

    delay(1000); // Update interval
  }
}

void moveForward() {
  analogWrite(enableA, 255);
  analogWrite(enableB, 255);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void turnLeft() {
  analogWrite(enableA, 255);
  analogWrite(enableB, 255);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void turnRight() {
  analogWrite(enableA, 255);
  analogWrite(enableB, 255);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

void stop() {
  analogWrite(enableA, 0);
  analogWrite(enableB, 0);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
