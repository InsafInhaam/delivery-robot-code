// A Robot - Code
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <TinyGPS++.h>

#define IN1 D1  // Motor 1
#define IN2 D2  // Motor 1
#define IN3 D3  // Motor 2
#define IN4 D4  // Motor 2

// Replace with your network credentials
const char* ssid = "Insaf-Inhaam-SLT-Fiber-4G";
const char* password = "InsafA@2003";

// Server URL to fetch GPS coordinates
const char* serverUrl = "http://192.168.1.27:8000/api/order/coordinates?robotId=66bc19c10f7288fb17f7a3a9";

// GPS and motor control
TinyGPSPlus gps;
HardwareSerial ss(1); // Use HardwareSerial instead of SoftwareSerial

double targetLat;
double targetLng;
const double distanceThreshold = 0.0001; // Adjust based on your required precision

void setup() {
  Serial.begin(115200);
  ss.begin(9600, SERIAL_8N1); // Initialize GPS at baud rate 9600
  delay(10);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Fetch new coordinates from the server
  fetchCoordinates();

  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  if (gps.location.isUpdated()) {
    double currentLat = gps.location.lat();
    double currentLng = gps.location.lng();

    Serial.print("Current Location: ");
    Serial.print(currentLat, 6);
    Serial.print(", ");
    Serial.println(currentLng, 6);

    if (distanceToTarget(currentLat, currentLng) > distanceThreshold) {
      moveTowardsTarget(currentLat, currentLng);
    } else {
      stopMotors();
      Serial.println("Arrived at destination.");
    }
  }
  
  delay(1000);
}

void fetchCoordinates() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverUrl);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Response:");
      Serial.println(payload);

      int latIndex = payload.indexOf("pickupLatitude") + 16;
      int lngIndex = payload.indexOf("pickupLongitude") + 17;
      targetLat = payload.substring(latIndex, payload.indexOf(",", latIndex)).toFloat();
      targetLng = payload.substring(lngIndex, payload.indexOf("}", lngIndex)).toFloat();
      
      Serial.print("Target Latitude: ");
      Serial.println(targetLat, 6);
      Serial.print("Target Longitude: ");
      Serial.println(targetLng, 6);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

void moveTowardsTarget(double currentLat, double currentLng) {
  double bearing = calculateBearing(currentLat, currentLng, targetLat, targetLng);
  double currentBearing = gps.course.deg();

  Serial.print("Bearing to target: ");
  Serial.println(bearing);
  Serial.print("Current bearing: ");
  Serial.println(currentBearing);

  if (abs(bearing - currentBearing) < 10) { // Adjust threshold as needed
    moveForward();
  } else if (bearing > currentBearing) {
    turnRight();
  } else {
    turnLeft();
  }
}

void moveForward() {
  Serial.println("Moving forward");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
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

double distanceToTarget(double lat1, double lng1) {
  const double R = 6371000;
  double dLat = radians(targetLat - lat1);
  double dLng = radians(targetLng - lng1);
  
  double a = sin(dLat / 2) * sin(dLat / 2) +
             cos(radians(lat1)) * cos(radians(targetLat)) *
             sin(dLng / 2) * sin(dLng / 2);
  
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  return R * c;
}

double calculateBearing(double lat1, double lng1, double lat2, double lng2) {
  double dLng = radians(lng2 - lng1);
  double y = sin(dLng) * cos(radians(lat2));
  double x = cos(radians(lat1)) * sin(radians(lat2)) - 
             sin(radians(lat1)) * cos(radians(lat2)) * cos(dLng);
  return degrees(atan2(y, x));
}
