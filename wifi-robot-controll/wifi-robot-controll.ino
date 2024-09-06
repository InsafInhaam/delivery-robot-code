#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

// Motor control pins
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4

// WiFi credentials
const char* ssid = "Insaf-Inhaam-SLT-Fiber-4G";
const char* password = "InsafA@2003";

AsyncWebServer server(80);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Handle web server routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<h1>NodeMCU Robot</h1><button onclick=\"location.href='/forward'\">Forward</button><button onclick=\"location.href='/backward'\">Backward</button><button onclick=\"location.href='/left'\">Left</button><button onclick=\"location.href='/right'\">Right</button><button onclick=\"location.href='/stop'\">Stop</button>");
  });

  server.on("/forward", HTTP_GET, [](AsyncWebServerRequest *request){
    forward();
    request->send(200, "text/html", "Moving Forward");
  });

  server.on("/backward", HTTP_GET, [](AsyncWebServerRequest *request){
    backward();
    request->send(200, "text/html", "Moving Backward");
  });

  server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request){
    left();
    request->send(200, "text/html", "Turning Left");
  });

  server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request){
    right();
    request->send(200, "text/html", "Turning Right");
  });

  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    stop();
    request->send(200, "text/html", "Stopping");
  });

  server.begin();
}

// Functions to control motors
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {
  // Nothing here, everything is handled by the web server
}
