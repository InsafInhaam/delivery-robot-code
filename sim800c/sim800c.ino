#include <SoftwareSerial.h>

SoftwareSerial sim800(7, 8); // RX, TX pins for SIM800 module

const char* apn = "your_apn"; // Replace with your cellular provider's APN
const char* serverUrl = "http://your-server-address/targets"; // Replace with your server URL

void setup() {
  Serial.begin(9600);
  sim800.begin(9600);

  // Initialize the module
  sendCommand("AT"); // Test AT command
  delay(1000);

  // Connect to the network
  sendCommand("AT+CGATT=1"); // Attach to the GPRS network
  delay(1000);

  // Set the APN
  sendCommand("AT+CGDCONT=1,\"IP\",\"" + String(apn) + "\"");
  delay(1000);

  // Connect to the internet
  sendCommand("AT+CGACT=1,1"); // Activate the GPRS context
  delay(1000);

  // Example: GET request to a server
  sendCommand("AT+HTTPINIT"); // Initialize HTTP
  delay(1000);
  sendCommand("AT+HTTPPARA=\"URL\",\"" + String(serverUrl) + "\"");
  delay(1000);
  sendCommand("AT+HTTPACTION=0"); // Send GET request
  delay(5000);
  sendCommand("AT+HTTPREAD"); // Read the response
  delay(5000);
  sendCommand("AT+HTTPTERM"); // Terminate HTTP
}

void loop() {
  // Add code to process responses or handle other tasks here
}

void sendCommand(String command) {
  sim800.println(command);
  while (sim800.available()) {
    Serial.write(sim800.read());
  }
}
