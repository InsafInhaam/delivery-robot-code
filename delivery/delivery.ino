// Motor pins
const int motorPin1 = 2;
const int motorPin2 = 3;
const int motorPin3 = 4;
const int motorPin4 = 5;
const int enableA = 9;
const int enableB = 10;

// Ultrasonic sensor pins
const int frontTrigPins[2] = {6, 7};
const int frontEchoPins[2] = {8, 9};
const int backTrigPins[2] = {10, 11};
const int backEchoPins[2] = {12, 13};
const int rightTrigPins[3] = {A0, A1, A2};
const int rightEchoPins[3] = {A3, A4, A5};
const int leftTrigPins[3] = {A6, A7, 14};
const int leftEchoPins[3] = {15, 16, 17};

// Light sensor pin
const int ldrPin = A8;

// LED pins
const int headlightPin = 18;
const int leftSignalPin = 19;
const int rightSignalPin = 20;
const int brakeLightPin = 21;

// Obstacle distance threshold (in cm)
const int threshold = 20;

// LDR threshold
const int ldrThreshold = 500;

void setup() {
  // Set motor pins as output
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);

  // Set ultrasonic sensor pins
  for (int i = 0; i < 2; i++) {
    pinMode(frontTrigPins[i], OUTPUT);
    pinMode(frontEchoPins[i], INPUT);
    pinMode(backTrigPins[i], OUTPUT);
    pinMode(backEchoPins[i], INPUT);
  }
  for (int i = 0; i < 3; i++) {
    pinMode(rightTrigPins[i], OUTPUT);
    pinMode(rightEchoPins[i], INPUT);
    pinMode(leftTrigPins[i], OUTPUT);
    pinMode(leftEchoPins[i], INPUT);
  }

  // Set LED pins as output
  pinMode(headlightPin, OUTPUT);
  pinMode(leftSignalPin, OUTPUT);
  pinMode(rightSignalPin, OUTPUT);
  pinMode(brakeLightPin, OUTPUT);

  // Start serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Check for obstacles
  bool obstacleFront = checkObstacle(frontTrigPins, frontEchoPins, 2);
  bool obstacleBack = checkObstacle(backTrigPins, backEchoPins, 2);
  bool obstacleRight = checkObstacle(rightTrigPins, rightEchoPins, 3);
  bool obstacleLeft = checkObstacle(leftTrigPins, leftEchoPins, 3);

  // Check light level
  int ldrValue = analogRead(ldrPin);
  if (ldrValue < ldrThreshold) {
    digitalWrite(headlightPin, HIGH);
  } else {
    digitalWrite(headlightPin, LOW);
  }

  // Control robot movement based on obstacles
  if (obstacleFront) {
    // Stop and turn right
    stop();
    delay(500);
    turnRight();
    delay(500);
  } else if (obstacleRight) {
    // Turn left
    stop();
    delay(500);
    turnLeft();
    delay(500);
  } else if (obstacleLeft) {
    // Turn right
    stop();
    delay(500);
    turnRight();
    delay(500);
  } else {
    // Move forward
    moveForward();
  }

  delay(100);
}

void moveForward() {
  digitalWrite(brakeLightPin, LOW); // Turn off brake light
  analogWrite(enableA, 255);
  analogWrite(enableB, 255);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void moveBackward() {
  digitalWrite(brakeLightPin, LOW); // Turn off brake light
  analogWrite(enableA, 255);
  analogWrite(enableB, 255);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

void turnLeft() {
  digitalWrite(brakeLightPin, LOW); // Turn off brake light
  analogWrite(enableA, 255);
  analogWrite(enableB, 255);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  blinkSignal(leftSignalPin); // Blink left signal
}

void turnRight() {
  digitalWrite(brakeLightPin, LOW); // Turn off brake light
  analogWrite(enableA, 255);
  analogWrite(enableB, 255);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  blinkSignal(rightSignalPin); // Blink right signal
}

void stop() {
  analogWrite(enableA, 0);
  analogWrite(enableB, 0);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  digitalWrite(brakeLightPin, HIGH); // Turn on brake light
}

bool checkObstacle(const int trigPins[], const int echoPins[], int sensorCount) {
  for (int i = 0; i < sensorCount; i++) {
    long duration, distance;
    digitalWrite(trigPins[i], LOW);
    delayMicroseconds(2);
    digitalWrite(trigPins[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[i], LOW);

    duration = pulseIn(echoPins[i], HIGH);
    distance = (duration / 2) / 29.1;

    if (distance < threshold) {
      return true;
    }
  }
  return false;
}

void blinkSignal(int signalPin) {
  digitalWrite(signalPin, HIGH);
  delay(500);
  digitalWrite(signalPin, LOW);
  delay(500);
}


