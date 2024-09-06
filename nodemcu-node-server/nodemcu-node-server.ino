#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Replace with your network credentials
const char* ssid = "Insaf-Inhaam-SLT-Fiber-4G";
const char* password = "InsafA@2003";

// Server URL to fetch GPS coordinates
const char* serverUrl = "http://192.168.1.27:8000/api/order/coordinates?robotId=66bc19c10f7288fb17f7a3a9";

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    // Create a WiFiClient object
    WiFiClient client;
    
    // Initialize HTTPClient
    HTTPClient http;
    
    Serial.println("Fetching data from server...");
    
    // Specify the URL with WiFiClient
    http.begin(client, serverUrl);
    
    // Send HTTP GET request
    int httpCode = http.GET();
    
    // Check the HTTP response code
    if (httpCode > 0) {
      // HTTP header has been sent and Server response header has been handled
      Serial.printf("HTTP GET request sent, response code: %d\n", httpCode);
      
      // Get the response payload (if any)
      String payload = http.getString();
      Serial.println("Response:");
      Serial.println(payload);
    } else {
      Serial.printf("GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    
    // End the HTTP connection
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  // Wait for 10 seconds before the next request
  delay(10000);
}
