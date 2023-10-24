#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "webpage.h"
#include "dialogues.h"
#include "web_handler_functions.h"
#include "ESP8266TimerInterrupt.h"

// Select a Timer Clock
#define USING_TIM_DIV1 false    // for shortest and most accurate timer
#define USING_TIM_DIV16 true    // for medium time and medium accurate timer
#define USING_TIM_DIV256 false  // for longest timer but least accurate. Default


long TIMER_INTERVAL_MS = 4000;

const int buzzer_pin = D2;
const int button_pin = D1;

const char* ssid = "Storm PTCL";
const char* password = "35348E80687?!";

long long last_beep_millis = 0;
long long threshold_millis = 0;

long long on_beep = 10;
long long off_beep = 1000;

String rawData = "36633";
String teamName = "Dev Board";
bool dialogReady = true;
bool scene_dialogue_completed = false;
bool scan_for_rssi =  false;

long long story_scene = 0;
long long scene_dialogue_count = 0;

String pastDialogue = "--Insert past dialogue here--";

// Init Server Object
ESP8266WebServer server(80);
// Init ESP8266 timer 1
ESP8266Timer ITimer;

int getStrengthOfSSID(String ssid_to_scan) {
  int numNetworks = WiFi.scanNetworks();
  if (numNetworks != 0) {
    for (int i = 0; i < numNetworks; i++) {
      if (WiFi.SSID(i) == ssid_to_scan) {
        int strength = WiFi.RSSI(i);
        return strength;
      }
    }
  }
  return -127;
}

double DBToLinear(long long val) {
  return (double)pow(10, val / 10.0);
}

bool isBeeping = false;

void IRAM_ATTR beep() {
  if (millis() - last_beep_millis > threshold_millis) {
    isBeeping = !isBeeping;
    digitalWrite(buzzer_pin, isBeeping);
    last_beep_millis = millis();
    isBeeping ? threshold_millis = on_beep : threshold_millis = off_beep;
  }
}

double mapDouble(double x, double in_min, double in_max, double out_min, double out_max) {
  return (double)(x - in_min) * (out_max - out_min) / (double)(in_max - in_min) + out_min;
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(button_pin, INPUT_PULLUP);
  pinMode(buzzer_pin, OUTPUT);

  digitalWrite(buzzer_pin, LOW);

  randomSeed(analogRead(A0));

  if (1) {
    // Connect to the "Storm PTCL" WiFi network with the specified password
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
    }
    Serial.println(".");
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

  if (ITimer.attachInterruptInterval(1000, beep)) {
    Serial.print("Starting ITimer OK");
  } else {
    Serial.println("Can't set ITimer correctly");
  }

  //ITimer.disableTimer();

  server.on("/", HTTP_GET, [&]() {
    handleRoot(server);
  });

  server.on("/bootTime", HTTP_GET, [&]() {
    handleBootTime(server);
  });

  server.on("/entered", HTTP_GET, [&]() {
    handleCMD(server, teamName, buzzer_pin);
  });

  server.on("/rawData", HTTP_GET, [&]() {
    handleRawData(server, rawData);
  });

  server.on("/dialogReady", HTTP_GET, [&]() {
    handleDialogReady(server, dialogReady, scene_dialogue_completed);
  });

  server.on("/pastDialogue", HTTP_GET, [&]() {
    handleAllDialogue(server, pastDialogue, scene_dialogue_completed);
  });

  server.on("/latestDialogue", HTTP_GET, [&]() {
    handleLatestDialogue(server, dialogues, buzzer_pin, story_scene, scene_dialogue_count, dialogues_count, dialogReady, scene_dialogue_completed);
  });

  server.begin();
  //dialogReady = true;
}

void loop() {
  if (scene_dialogue_completed == true) {
    if (scan_for_rssi == true) {
      //ITimer.enableTimer();
      int signalStrength = getStrengthOfSSID("HP-LASERJET-1881");

      long duration = mapDouble(max(signalStrength, -80), -80, -20, 2000, 1);  // Map signal strength to duration
      off_beep = duration;

      //ITimer.setInterval(duration * 1000, beep);
      Serial.println(duration);


      if (signalStrength <= -20) {
        // Start beeping
        //isBeeping = true;
      } else {
        // Stop beeping when signal strength is greater than -20
        isBeeping = false;
        dialogReady = true;
        scan_for_rssi = false;
      }
    }

    if (digitalRead(button_pin) == LOW) {
      dialogReady = true;
    }
  }
  server.handleClient();
}
