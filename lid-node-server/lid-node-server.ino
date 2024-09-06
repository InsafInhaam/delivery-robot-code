#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

// Replace with your network credentials
const char* ssid = "Insaf-Inhaam-SLT-Fiber-4G";
const char* password = "InsafA@2003";

// Server URL to fetch the lid control command
const char* serverUrl = "http://192.168.1.27:8000/api/robot/lid?robotId=66c0949fde0ddc7f283ca0cb";

// Define Servo and pin
Servo lidServo;
const int servoPin = D7; // PWM-capable pin for servo

void setup() {
  Serial.begin(115200);
  delay(10);

  // Initialize the servo motor
  lidServo.attach(servoPin);

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
  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    // Fetch the command from the server
    fetchLidCommand();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(10000); // Check every 10 seconds (adjust as needed)
}

void fetchLidCommand() {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverUrl);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Lid Command Response:");
    Serial.println(payload);

    if (payload == "open") {
      openLid();
    } else if (payload == "close") {
      closeLid();
    } else {
      Serial.println("Unknown command");
    }
  } else {
    Serial.printf("GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void openLid() {
  Serial.println("Opening lid...");
  lidServo.write(0); // Adjust the angle as needed
}

void closeLid() {
  Serial.println("Closing lid...");
  lidServo.write(180); // Adjust the angle as needed
}
