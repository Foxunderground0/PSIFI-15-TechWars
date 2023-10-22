#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "webpage.h"
#include "web_handler_functions.h"

const int buzzer_pin = D2;
const int button_pin = D1;

const char* ssid = "Storm PTCL";
const char* password = "35348E80687?!";

ESP8266WebServer server(80);

String rawData = "36633";
String teamName = "Dev Board";
bool dialogReady = true;
bool scene_dialogue_completed = false;
int story_scene = 0;
int scene_dialogue_count = 0;

String pastDialogue = "--Insert past dialogue here--";

int dialogues_count[] = { 4, 2 };
String dialogues[][4] = {
  { "Well, well, well, look who's decided to join the fun. I'm Cardy, your oh-so-charming robotic companion for this little escapade. Consider yourselves lucky.", "In case you haven't figured it out, this is no ordinary scavenger hunt. You're in for a challenge, or two, or a dozen. Don't expect any hand-holding, my friends. You're on your own here.", "I'll be your source of cryptic clues and enigmatic riddles. But whether you can actually solve them is another matter entirely. You see, I've got a reputation for being a bit, shall we say, sarcastic.", "So, gather your wit, grit, and maybe a bit of luck. It's time to test your mettle and see if you can keep up. Just remember, you asked for this. Let the games begin, and may the snarkiest team win!" },
  { "Second scene dialogue 1", "Second scene dialogue 2", "Second scene dialogue 3", "Second scene dialogue 4" }
};


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

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(button_pin, INPUT_PULLUP);
  pinMode(buzzer_pin, OUTPUT);

  digitalWrite(buzzer_pin, LOW);

  randomSeed(analogRead(A0));

  if (0) {
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
    handleDialogReady(server, dialogReady);
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
bool isBeeping = true;

void beep(int pin, unsigned int frequency, unsigned long duration_on, unsigned long duration_off) {
  if (isBeeping) {
    tone(pin, frequency);
    delay(duration_on);
    noTone(pin);
    delay(duration_off);
  }
}

void loop() {
  if (scene_dialogue_completed == true) {
    int signalStrength = getStrengthOfSSID("HP-LASERJET-1881");
    Serial.println(signalStrength);

    int frequency = map(signalStrength, -127, -20, 100, 2000);  // Map signal strength to frequency
    int duration_on = map(signalStrength, -127, -20, 500, 10);    // Map signal strength to duration
    int duration_off = map(signalStrength, -127, -20, 5000, 0);    // Map signal strength to duration

    if (signalStrength <= -20) {
      // Start beeping
      //isBeeping = true;
    } else {
      // Stop beeping when signal strength is greater than -20
      isBeeping = false;
      dialogReady = true;
    }

    if (isBeeping) {
      beep(buzzer_pin, frequency, duration_on, duration_off);
    } else {
      // Stop beeping
      noTone(buzzer_pin);
    }
  }

  if (digitalRead(button_pin) == LOW) {
    // Button pressed, trigger beeping
    isBeeping = true;
  }

  server.handleClient();
}
