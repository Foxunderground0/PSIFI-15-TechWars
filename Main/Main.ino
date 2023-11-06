#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "webpage.h"
#include "dialogues.h"
#include "web_handler_functions.h"
#include "ESP8266TimerInterrupt.h"
#include <MD_MAX72xx.h>

#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW
#define MAX_DEVICES 1

#define CLK_PIN D5   //14 or SCK
#define DATA_PIN D7  //13 or MOSI
#define CS_PIN D6    //12 or SS


// Select a Timer Clock
#define USING_TIM_DIV1 false    // for shortest and most accurate timer
#define USING_TIM_DIV16 true    // for medium time and medium accurate timer
#define USING_TIM_DIV256 false  // for longest timer but least accurate. Default

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

long TIMER_INTERVAL_MS = 4000;

const int buzzer_pin = 4;
const int button_pin = D1;

const char* ssid = "Storm PTCL";
const char* password = "35348E80687?!";

long long last_beep_millis = 0;
long long threshold_millis = 0;

long long on_beep = 20;
long long off_beep = 1000;

String rawData = "36633";
String teamName = "Dev Board";
bool dialogReady = true;
bool scene_dialogue_completed = false;
bool scan_for_rssi = true;

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
  //Serial.begin(115200);
  //Serial.println();

  pinMode(button_pin, INPUT_PULLUP);
  pinMode(buzzer_pin, OUTPUT);

  digitalWrite(buzzer_pin, LOW);

  randomSeed(analogRead(A0));

  // WIFI CONFIG
  if (1) {
    // Connect to the "Storm PTCL" WiFi network with the specified password
    WiFi.begin(ssid, password);
    //Serial.print("Connecting to WiFi ");
    while (WiFi.status() != WL_CONNECTED) {
      //Serial.print(".");
      delay(100);
    }
    //Serial.println(".");
    //Serial.println("Connected to WiFi");
    //Serial.print("IP address: ");
    //Serial.println(WiFi.localIP());
  } else {
    // Configure the Access Point
    WiFi.softAP("Test", "");

    // Get the IP address of the Access Point
    IPAddress apIP = WiFi.softAPIP();
    //Serial.print("Access Point IP address: ");
    //Serial.println(apIP);
  }

  // MATRIX BEGIN
  mx.begin();

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      mx.setPoint(i, j, true);
    }
  }

  // OTA CONFIG
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    //Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    //Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    //Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      //Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      //Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      //Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      //Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      //Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();

  // TIMER CONFIG
  if (ITimer.attachInterruptInterval(1000, beep)) {
    //Serial.print("Starting ITimer OK");
  } else {
    //Serial.println("Can't set ITimer correctly");
  }

  //ITimer.disableTimer();

  // SERVER CONFIG
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

      long duration = mapDouble(max(signalStrength, -90), -90, -20, 2000, 1);  // Map signal strength to duration
      mx.control(MD_MAX72XX::INTENSITY, map(max(signalStrength, -90), -90, -20, 0x0, 0xf));
      off_beep = duration;

      //ITimer.setInterval(duration * 1000, beep);
      //Serial.println(duration);


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
  ArduinoOTA.handle();
}
