// Define motor pins for Arduino Mega using one motor driver
#define IN1  8   // Motor 1
#define IN2  9   // Motor 1
#define IN3  10  // Motor 2
#define IN4  11  // Motor 2

// Define ultrasonic sensor pins
#define TRIG_FRONT  2   // Front sensor
#define ECHO_FRONT  3   // Front sensor
#define TRIG_BACK   4   // Back sensor
#define ECHO_BACK   5   // Back sensor
#define TRIG_RIGHT  6   // Right sensor
#define ECHO_RIGHT  7   // Right sensor
#define TRIG_LEFT   12  // Left sensor
#define ECHO_LEFT   13  // Left sensor

const int distanceThreshold = 20; // Distance threshold in cm for obstacle detection

void setup() {
  // Initialize the motor pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Initialize the ultrasonic sensor pins
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_BACK, OUTPUT);
  pinMode(ECHO_BACK, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);
  
  // Start serial communication at 9600 baud
  Serial.begin(9600);
}

void loop() {
  if (!isObstacleFront()) {
    moveForward();
  } else if (!isObstacleBack()) {
    moveBackward();
  } else if (!isObstacleRight()) {
    turnRight();
  } else if (!isObstacleLeft()) {
    turnLeft();
  } else {
    stopMotors();
  }
  
  delay(500); // Adjust delay as needed for smoother movement
}

void moveForward() {
  Serial.println("Moving forward");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  Serial.println("Moving backward");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  Serial.println("Turning left");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  Serial.println("Turning right");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  Serial.println("Stopping motors");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Function to measure distance using an ultrasonic sensor
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration / 2) / 29.1; // Convert time to distance in cm
  return distance;
}

// Check for obstacles
bool isObstacleFront() {
  long distance = measureDistance(TRIG_FRONT, ECHO_FRONT);
  Serial.print("Front distance: ");
  Serial.println(distance);
  return (distance < distanceThreshold);
}

bool isObstacleBack() {
  long distance = measureDistance(TRIG_BACK, ECHO_BACK);
  Serial.print("Back distance: ");
  Serial.println(distance);
  return (distance < distanceThreshold);
}

bool isObstacleRight() {
  long distance = measureDistance(TRIG_RIGHT, ECHO_RIGHT);
  Serial.print("Right distance: ");
  Serial.println(distance);
  return (distance < distanceThreshold);
}

bool isObstacleLeft() {
  long distance = measureDistance(TRIG_LEFT, ECHO_LEFT);
  Serial.print("Left distance: ");
  Serial.println(distance);
  return (distance < distanceThreshold);
}
