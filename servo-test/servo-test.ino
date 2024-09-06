#include <Servo.h>

#define SERVO_PIN 9  // Define the pin where the servo motor is connected

Servo myServo;

void setup() {
  Serial.begin(9600);  // Start serial communication for debugging purposes
  myServo.attach(SERVO_PIN);  // Attach the servo on the specified pin
  Serial.println("Servo test");

  // Move the servo to 0 degrees
  myServo.write(0);
  Serial.println("Moved to 0 degrees");
  delay(1000);

  // Move the servo to 90 degrees
  myServo.write(90);
  Serial.println("Moved to 90 degrees");
  delay(1000);

  // Move the servo to 180 degrees
  myServo.write(180);
  Serial.println("Moved to 180 degrees");
  delay(1000);

  // Move the servo back to 90 degrees
  myServo.write(90);
  Serial.println("Moved back to 90 degrees");
  delay(1000);

  // Move the servo back to 0 degrees
  myServo.write(0);
  Serial.println("Moved back to 0 degrees");
}

void loop() {
  // The loop can be left empty as the setup already tests the servo
}
