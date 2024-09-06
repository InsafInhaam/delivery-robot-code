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
  // Test Motor 1
  Serial.println("Testing Motor 1 - Forward");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(5000); // Move Motor 1 forward for 5 seconds
  
  Serial.println("Testing Motor 1 - Backward");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(5000); // Move Motor 1 backward for 5 seconds
  
  Serial.println("Stopping Motor 1");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(5000); // Stop for 5 seconds

  // Test Motor 2
  Serial.println("Testing Motor 2 - Forward");
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(5000); // Move Motor 2 forward for 5 seconds
  
  Serial.println("Testing Motor 2 - Backward");
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(5000); // Move Motor 2 backward for 5 seconds
  
  Serial.println("Stopping Motor 2");
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(5000); // Stop for 5 seconds
}
