#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Create a SoftwareSerial object for GPS
SoftwareSerial ss(4, 3); // RX, TX

// Create a TinyGPS++ object
TinyGPSPlus gps;
 
void setup() {
  // Start the serial communication
  Serial.begin(9600);
  ss.begin(9600);
  
  Serial.println("NEO-6M GPS Module Test");
  Serial.println("Waiting for GPS data...");
}

void loop() {
  while (ss.available() > 0) {
    gps.encode(ss.read());

    if (gps.location.isUpdated()) {
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);
      Serial.print("Altitude: ");
      Serial.println(gps.altitude.meters());
      Serial.print("Satellites: ");
      Serial.println(gps.satellites.value());
      Serial.print("Speed: ");
      Serial.println(gps.speed.kmph());
      Serial.println();
    }
  }
}
