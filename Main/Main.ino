#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include "dialogues.h"
#include "web_handler_functions.h"
#include "file_handler_functions.h"
#include "ESP8266TimerInterrupt.h"
#include "utils.h"
#include <MD_MAX72xx.h>
#include <LittleFS.h>

#define TEST 0                   // Enable Testing Of Hardware
#define STATION_MODE_SELECTOR 1  // WIFI Acesspoint Modes
#define SERIAL_ENABLE 0

#define  TEXTSCROLLDELAY  100  // in milliseconds
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

long TIMER_INTERVAL_MS = 4000;  //4ms

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
bool scan_for_rssi = false;
bool isBeeping = false;
bool isGame = true; // Serves JS game at the start

long long story_scene = 0;
long long scene_dialogue_count = 0;

long ledRunIndex = 0;
bool ledRunState = true;

const String dialogue_file_path = "/dialogue_persist.bin";

String pastDialogue = "--Insert past dialogue here--";

// Init Server Object
ESP8266WebServer server(80);
// Init ESP8266 timer 1
ESP8266Timer ITimer;
// Create an instance of the HTTPUpdateServer class
ESP8266HTTPUpdateServer httpUpdater;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void scrollText(const char* p) {
  uint8_t charWidth;
  uint8_t cBuf[8];  // this should be ok for all built-in fonts

  mx.clear();

  while (*p != '\0')
  {
    charWidth = mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);

    for (uint8_t i = 0; i <= charWidth; i++)  // allow space between characters
    {
      mx.transform(MD_MAX72XX::TSL);
      mx.setColumn(0, (i < charWidth) ? cBuf[i] : 0);
      delay(TEXTSCROLLDELAY);
    }
  }
}

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

// Beep the buzzer, and alternate the threshold millis depending if currently on beep or off beep
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

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  SerialPrintLn("Connected to Wi-Fi sucessfully.");
  SerialPrint("IP address: ");
  SerialPrintLn(WiFi.localIP());
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  SerialPrintLn("Disconnected from Wi-Fi, trying to connect...");
  WiFi.disconnect();
  WiFi.begin(ssid, password);
}

void setup() {

#if SERIAL_ENABLE
  Serial.begin(115200);
#endif

  SerialPrintLn();

#if STATION_MODE_SELECTOR == 1
  //Register wifi event handlers
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
#endif

  pinMode(button_pin, INPUT_PULLUP);
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(buzzer_pin, LOW);

  randomSeed(analogRead(A0));

  // MATRIX BEGIN
  mx.begin();

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      mx.setPoint(ledRunIndex / 8, ledRunIndex % 8, ledRunState);
      ledRunIndex++;
    }
  }
  ledRunIndex = 0;

  // WIFI CONFIG
  if (STATION_MODE_SELECTOR) {
    // Connect to the "Storm PTCL" WiFi network with the specified password
    WiFi.begin(ssid, password);
    SerialPrint("Connecting to WiFi ");
    while (WiFi.status() != WL_CONNECTED) {
      SerialPrint(".");
      scrollText("Booting");
      //delay(100);
    }

    SerialPrintLn(".");
    SerialPrintLn("Connected to WiFi");
    SerialPrint("IP address: ");
    SerialPrintLn(WiFi.localIP());
  } else {
    // Configure the Access Point
    WiFi.softAP("Test", "");

    // Get the IP address of the Access Point
    IPAddress apIP = WiFi.softAPIP();
    SerialPrint("Access Point IP address: ");
    SerialPrintLn(apIP);
  }

  // File system initialised
  if (!LittleFS.begin()) {
    SerialPrintLn("LittleFS initialization failed!");
  } else {
    SerialPrintLn("LittleFS OK");
  }

  if (!checkIfAllDataFileExists()) {
    SerialPrintLn("Not all required files are on little FS!");
  } else {
    SerialPrintLn("Data Files on Little FS OK");
  }

  SerialPrintLn("Reading presistant data");
  if (fileExists(dialogue_file_path)) {
    readPersistedDialogue(story_scene, scene_dialogue_count);
    SerialPrintLn("File exists. Values read from file:");
    SerialPrintLn("story_scene: " + String(story_scene));
    SerialPrintLn("scene_dialogue_count: " + String(scene_dialogue_count));
  } else {
    // If the file doesn't exist, create it with initial data "0,0"
    updatePersistedDialogue(0LL, 0LL);
    SerialPrintLn("File doesn't exist. Created with initial values 0, 0.");
  }
  SerialPrintLn("File reading done");

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
    SerialPrintLn("Start updating " + type);
    });
  ArduinoOTA.onEnd([]() {
    SerialPrintLn("\nEnd");
    });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      SerialPrintLn("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      SerialPrintLn("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      SerialPrintLn("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      SerialPrintLn("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      SerialPrintLn("End Failed");
    }
    });

  ArduinoOTA.begin();
  SerialPrintLn("OTA OK");

  // TIMER CONFIG
  if (ITimer.attachInterruptInterval(1000, beep)) {
    SerialPrintLn("ITimer OK");
  } else {
    SerialPrintLn("Can't set ITimer correctly");
  }
  //ITimer.disableTimer();

  // SERVER CONFIG
  server.on("/", HTTP_GET, [&]() {
    handleRoot(server, dialogReady, isGame);
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
    handlePastDialogue(server, dialogues, scene_dialogue_completed, story_scene, scene_dialogue_count);
    });

  server.on("/latestDialogue", HTTP_GET, [&]() {
    handleLatestDialogue(server, dialogues, buzzer_pin, story_scene, scene_dialogue_count, dialogues_count, dialogReady, scene_dialogue_completed, scan_for_rssi);
    });

  server.on("/littleFS", HTTP_GET, [&]() {
    handleFSContent(server, dialogue_file_path);
    });

  server.on("/video", HTTP_GET, [&]() {
    handleMKV(server);
    });

  server.on("/reset", HTTP_GET, [&]() {
    ESP.reset();
    });

  server.on("/verified", HTTP_GET, [&]() {
    isGame = false;
    server.sendHeader("Location", "/", true);  // Set the "Location" header to "/"
    server.send(308, "text/plain", "");        // Respond with a 308 status code
    });



  //server.on("/aa", HTTP_GET, [&]() {
  //  server.send(200, "text/plain", String(ledRunIndex) + " " + String(ledRunState));
  //});

  server.begin();
  SerialPrintLn("Web Server OK");

  // Setup the HTTPUpdateServer
  httpUpdater.setup(&server);
  SerialPrintLn("HTTP OTA Update Server OK");

  MDNS.begin("esp8266");
  SerialPrintLn("MDNS Server OK");

  // Output Chip ID
  SerialPrint("Chip ID: 0x");
  SerialPrintLn2(ESP.getChipId(), HEX);

  if (TEST) {
    //Test Buzzer
    digitalWrite(buzzer_pin, HIGH);
    delay(100);
    digitalWrite(buzzer_pin, LOW);

    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        mx.setPoint(i, j, true);
        delay(10);
      }
    }

    for (char a = 0; a < 0xf; a++) {
      mx.control(MD_MAX72XX::INTENSITY, a);
      delay(50);
    }

    for (char a = 0xf; a > 0x0; a--) {
      mx.control(MD_MAX72XX::INTENSITY, a);
      delay(50);
    }

    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        mx.setPoint(i, j, false);
        delay(20);
      }
    }

    scrollText(("TECHWARS - CARDY" + String(micros())).c_str());
  }

  // Set matrix to all off
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      mx.setPoint(i, j, false);
    }
  }

  SerialPrint("Boot Completed in: ");
  SerialPrint(millis());
  SerialPrint("ms ");
  SerialPrint(micros());
  SerialPrintLn("us");

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
      SerialPrintLn(duration);


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
      digitalWrite(buzzer_pin, HIGH);
      delay(100);
      digitalWrite(buzzer_pin, LOW);
      dialogReady = true;
    }
  }

  server.handleClient();
  ArduinoOTA.handle();

  mx.setPoint(ledRunIndex / 8, ledRunIndex % 8, ledRunState);
  ledRunIndex++;

  if (ledRunIndex > 63) {
    ledRunIndex = 0;
    ledRunState = !ledRunState;
  }

  for (char a = 0; a < 0xf; a++) {
    mx.control(MD_MAX72XX::INTENSITY, a);
    delay(5);
  }

  for (char a = 0xf; a > 0x0; a--) {
    mx.control(MD_MAX72XX::INTENSITY, a);
    delay(5);
  }

  //SerialPrintLn(String(micros()));// + " " + String(scene_dialogue_count));
}
