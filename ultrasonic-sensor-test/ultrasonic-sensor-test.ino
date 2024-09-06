#include <Ultrasonic.h>

const int trigPin = 8;  // Replace with your actual trigger pin
const int echoPin = 9;  // Replace with your actual echo pin

Ultrasonic rightSensor(trigPin, echoPin);

void setup() {
  Serial.begin(9600);
}

void loop() {
  long distance = rightSensor.read();  // Reading distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(500);
}
