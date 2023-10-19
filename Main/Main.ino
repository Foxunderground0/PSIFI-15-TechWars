#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <base64.h>
#include <LittleFS.h>
#include "webpage.h"

const char* ssid = "Storm PTCL";
const char* password = "35348E80687?!";

ESP8266WebServer server(80);

String rawData = "36633";
void handleRoot() {
  // Create a simple HTML page that displays the rawData and updates it every second

  // Base64-decode the HTML content


  //server.sendHeader("Location", base64Data);
  server.send(200, "text/html", htmlContent, sizeof(htmlContent));
}

void handleRawData() {
  // Respond with the current value of rawData
  server.send(200, "text/plain", rawData);
}

void handleCMD() {
  Serial.println(server.arg("command"));
  server.send(200, "text/plain", "BASED");
  digitalWrite(D1, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(50);                      // wait for a second
  digitalWrite(D1, LOW);   // turn the LED off by making the voltage LOW
  
}

void bootTime() {
  // Respond with the current value of rawData
  server.send(200, "text/plain", String(millis()));
}


long getStrengthOfSSID(String ssid_to_scan) {
  int numNetworks = WiFi.scanNetworks();
  if (numNetworks != 0) {
    for (int i = 0; i < numNetworks; i++) {
      if (WiFi.SSID(i) == ssid_to_scan) {
        long strength = WiFi.RSSI(i);
        Serial.print(strength);
        return strength;
      }
    }
  }
  return -1;
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW);

  if (0) {
    // Connect to the "Storm PTCL" WiFi network with the specified password
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

  } else {
    // Configure the Access Point
    WiFi.softAP("Test", "");

    // Get the IP address of the Access Point
    IPAddress apIP = WiFi.softAPIP();
    Serial.print("Access Point IP address: ");
    Serial.println(apIP);
  }

  if (!LittleFS.begin()) {
    Serial.println("LittleFS initialization failed!");
    return;
  }
  Serial.println("LittleFS initialized successfully.");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/rawData", HTTP_GET, handleRawData);
  server.on("/entered", HTTP_GET, handleCMD);
  server.on("/bootTime", HTTP_GET, bootTime);

  server.begin();
}

void loop() {
  //rawData = String(getStrengthOfSSID("HP-LASERJET-1881"));
  server.handleClient();
}
