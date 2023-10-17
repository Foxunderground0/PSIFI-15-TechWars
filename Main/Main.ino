#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
int messageCounter = 0;

void handleRoot() {
  server.send(200, "text/html", "<!DOCTYPE html><html><head><title>SSE Example</title></head><body style=\"background-color:DodgerBlue;\"><div id=\"message-container\"></div><script>const messageContainer = document.getElementById(\"message-container\");const eventSource = new EventSource(\"/sse\");eventSource.onmessage = function(event) {const message = event.data;const newMessage = document.createElement('div');newMessage.textContent = 'Received: ' + message;messageContainer.appendChild(newMessage);};eventSource.onerror = function(event) {console.error('SSE error:', event);};</script></body></html>");
}

void handleSSE() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/event-stream");
  server.client().setNoDelay(true);

  while (true) {
    String message = "Message " + String(messageCounter++);
    server.sendContent("data: " + message + "\n\n");
    delay(1000); // Wait for 10 seconds before sending the next message
  }
}

void setup() {
  Serial.begin(115200);
  
  // Create a hidden WiFi hotspot with SSID "Test" and no password
  WiFi.softAP("Test", "", 1, 0); // The third argument (1) makes the SSID hidden
  
  Serial.println("Hidden WiFi hotspot 'Test' created");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  
  server.on("/", HTTP_GET, handleRoot);
  server.on("/sse", HTTP_GET, handleSSE);

  server.begin();
}

void loop() {
  server.handleClient();
}