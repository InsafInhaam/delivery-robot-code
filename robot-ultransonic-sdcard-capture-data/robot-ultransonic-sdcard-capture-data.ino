#include <SPI.h>
#include <SD.h>
#include <Ultrasonic.h>

// Define pins for ultrasonic sensors
Ultrasonic frontSensor(2, 3);  // Front sensor: Trig Pin 2, Echo Pin 3
Ultrasonic backSensor(4, 5);   // Back sensor: Trig Pin 4, Echo Pin 5
Ultrasonic leftSensor(6, 7);   // Left sensor: Trig Pin 6, Echo Pin 7
Ultrasonic rightSensor(8, 9);  // Right sensor: Trig Pin 8, Echo Pin 9

const int chipSelect = 10;
File dataFile;

void setup() {
  Serial.begin(9600);
  
  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");
  
  // Open or create the file
  dataFile = SD.open("data.txt", FILE_WRITE);
  if (!dataFile) {
    Serial.println("Failed to open file for writing.");
  }
}

void loop() {
  // Read distances from ultrasonic sensors
  float frontDistance = frontSensor.read();
  float backDistance = backSensor.read();
  float leftDistance = leftSensor.read();
  float rightDistance = rightSensor.read();
  
  // Determine environment label based on sensor readings
  String label;
  if (frontDistance > 100 && backDistance > 100 && leftDistance > 100 && rightDistance > 100) {
    label = "open space";
  } else if (frontDistance < 50 || backDistance < 50 || leftDistance < 50 || rightDistance < 50) {
    label = "near wall";
  } else if ((leftDistance < 50 && frontDistance < 50) || (rightDistance < 50 && frontDistance < 50)) {
    label = "corner";
  } else {
    label = "unknown";
  }
  
  // Log data to Serial Monitor
  Serial.print("Label: ");
  Serial.print(label);
  Serial.print(", Front: ");
  Serial.print(frontDistance);
  Serial.print(" cm, Back: ");
  Serial.print(backDistance);
  Serial.print(" cm, Left: ");
  Serial.print(leftDistance);
  Serial.print(" cm, Right: ");
  Serial.println(rightDistance);
  
  // Log data to SD card with label
  if (dataFile) {
    dataFile.print("Label: ");
    dataFile.print(label);
    dataFile.print(", Front: ");
    dataFile.print(frontDistance);
    dataFile.print(" cm, Back: ");
    dataFile.print(backDistance);
    dataFile.print(" cm, Left: ");
    dataFile.print(leftDistance);
    dataFile.print(" cm, Right: ");
    dataFile.println(rightDistance);
    dataFile.flush();  // Ensure data is written to the file
  }

  delay(500);  // Wait before the next reading
}

void closeFile() {
  if (dataFile) {
    dataFile.close();  // Close the file when done
  }
}
