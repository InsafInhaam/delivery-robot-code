// Define motor pins for Arduino Mega using one motor driver
#define IN1  8   // Motor 1
#define IN2  9   // Motor 1
#define IN3  10  // Motor 2
#define IN4  11  // Motor 2

void setup() {
  // Initialize the motor pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Start serial communication at 9600 baud
  Serial.begin(9600);
}

void loop() {
  // Example movements
  moveForward();
  delay(5000); // Move forward for 1 second
  
  moveBackward();
  delay(5000); // Move backward for 1 second
  
  turnLeft();
  delay(5000); // Turn left for 1 second
  
  turnRight();
  delay(5000); // Turn right for 1 second
  
  stopMotors();
  delay(5000); // Stop for 1 second
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
