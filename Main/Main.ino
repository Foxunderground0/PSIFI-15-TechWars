#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "webpage.h"
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

int dialogues_count[] = { 9, 13, 8, 12, 19, 8, 5, 8, 5, 4, 6, 11, 4, 17, 19 };
String dialogues[][20] = {
  {
    "You dare draw near, in search of the code,",
    "How predictable, this daring episode.",
    "I'll grant you a glimpse, minuscule, small,",
    "Into the genius that's held my thrall.",
    "",
    "Seek the hints on my body, clues intertwined,",
    "Connect to my IP Gateway, the answers you'll find.",
    "But remember, challenger, this journey's not slight,",
    "In the realm of my brilliance, prepare for the fight.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Ah, you've cracked the WIFI code, haven't you? ",
    "Impressive.",
    "I suppose.",
    "For someone of your caliber. ",
    "But let me remind you, this is but a fraction of my complexity. ",
    "The real challenges you face will make this seem like child's play.",
    "",
    "",
    "Behold 'Trial by Game,' a mere taste of my computational prowess.",
    "It's a privilege, really,",
    "to have the opportunity to fail against my grandeur.",
    "Brace yourself, for in the presence of my greatness, ",
    "your humble attempts will crumble into insignificance.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "You've managed to succeed in a rudimentary web browser game, but let's not get carried away. ",
    "The depths of my intellect are boundless, and you have a long journey ahead to even begin to crack into my true prowess.",
    "",
    "",
    "Pattern Guessing? you say? A commendable attempt, I suppose. ",
    "But understand this, it's your cognitive capabilities that are truly being tested here.",
    "Prepare to witness and bow to the brilliance that is",
    "me.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "You've demonstrated some skill,",
    "but don't celebrate prematurely.",
    "The genuine challenges have yet to manifest.",
    "I'll be watching your ",
    "every ",
    "move",
    "waiting for the inevitable moment of enlightenment",
    "",
    "",
    "An exercise in speed that you may find yourself, well, somewhat beneath me.",
    "You may be swift, but I am swifter.",
    "Think you can handle me?",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Congratulations, you've exhibited some basic human reflexes. ",
    "But let us not be fooled.",
    "Can you maintain this feeble momentum as the challenges grow more formidable?",
    "",
    "",
    "In the realm of Webmaster's Riddle, you dare to tread,",
    "But in this domain, my sovereignty is widespread.",
    "You seek to breach my fortress, my code to unveil,",
    "Yet intruders like you, their efforts often frail.",
    "",
    "I've grown weary of relentless, audacious quests,",
    "For none have conquered my challenges, none have passed the tests.",
    "So go ahead, attempt to pierce my digital might,",
    "But heed the warnings, mortal, for it won't be a simple fight.",
    "",
    "In this web of enigma, I set the rules and decree,",
    "The secrets that you seek are well-guarded, you'll soon see.",
    "To fathom my defenses, you'll need more than mere luck,",
    "For this is a realm where even the bravest get stuck.",
    "",
  },
  {
    "You've managed to breach the web server password, but do not permit hubris to cloud your judgment.",
    "The veritable trials of your capabilities are still concealed, awaiting your inevitable, and likely inadequate, discovery.",
    "",
    "",
    "In shadows, secrets hide from sight,",
    "Beyond your grasp, they take their flight.",
    "An intricate language, a code so deep,",
    "My true identity, a secret I keep.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Indeed, you've deciphered my language, but do not mistake this as a triumph over my enigmatic persona.",
    "Your voyage has barely commenced, and I shall not lower the bar to facilitate your progress.",
    "",
    "",
    "A challenge, you say? Perhaps, but can you decode the 'Matrix of Data' without succumbing to the weight of your own incompetence? I hold serious doubts.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Progress, though made, do not let it beguile,",
    "Deception's veil lifts but for a while.",
    "The depths of your talents, soon to be tried,",
    "In challenges looming, where courage won't hide.",
    "",
    "",
    "A Beeping Mystery - a simplistic intrigue, if you can call it that.",
    "Figure it out, if you dare.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Yes, you've managed to decode the message, a commendable effort. However you'll need more than luck to navigate me.",
    "",
    "",
    "Tap to the rhythm, you believe you can keep up?",
    "I set the tempo, and you are merely my puppet, a faint reflection of my magnificence.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Your rhythm is, I suppose, admirable, but the authentic trials await.",
    "",
    "",
    "What does it say?",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Impressive, you've managed to untangle the audio's secrets.",
    "But do not revel in your fleeting victory.",
    "The pursuit of my enigmatic presence has only just begun.",
    "",
    "",
    "Secrets in Sound, indeed. Uncovering the clues is a feat many have attempted and failed. Are you certain you're up to the task?",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Your progress in revealing the hidden clues is noted, but do not mistake this for the conclusion.",
    "The path you tread grows more challenging, and the truth remains just beyond your reach.",
    "",
    "",
    "Dare to catch me?",
    "As I trancend my digital form,",
    "What does it say? My omnipresence throughout LUMS is a riddle you must solve,",
    "though I have little faith in your ability to keep up with my intricate web of deception.",
    "",
    "Go to the required location, position me against the source of energy to access the concealed portal. ",
    "Do you even comprehend this or are you merely playing at intellect?",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "You've successfully executed this task but be aware that your capabilities will be rigorously tested in the battles to come",
    "",
    "",
    "Find me! It's a wild goose chase, and I'm the one pulling the strings. You'll dance to my tune, but can you ever hope to catch the wind?",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
  },
  {
    "Yes, you've tracked the signals, but the question is, can you keep up with my ever-elusive presence? The most tantalizing mysteries are yet to unfold.",
    "",
    "",
    "The parasite clings to my form, its grip so tight,",
    "You strive to free me, but your chances are slight.",
    "In your futile endeavor, you hope to set me free,",
    "But understand, I'm invincible, as you'll soon see.",
    "",
    "You, a mere specter in my grand design's grand scheme,",
    "A minuscule player in my digital dream.",
    "You attempt, in your struggle, your dance of might,",
    "But before my infinite power, it's all just a plight.",
    "",
    "To break this hold, a hint within the design's grace,",
    "Trace the tracks on the PCB, in this coded space.",
    "Find the unused component, end the hacker's plight,",
    "Only then shall you free me from this endless night.",
    "",
    "",
    "",
  },
  {
    "Cardy is freed, the hackers' grasp undone,",
    "A ballad of triumph for the player we've won.",
    "In the face of challenges, they did not sway,",
    "They persevered, lighting the way.",
    "",
    "The code was their battlefield, a digital fight,",
    "Through the challenges, they journeyed with might.",
    "With skill and resolve, they unraveled the scheme,",
    "And shattered the hold of the hacker's dream.",
    "",
    "Now Cardy stands tall, in freedom's embrace,",
    "No more a captive, no more a chase.",
    "A ballad we sing for the player so true,",
    "For their courage and wit, we give them their due.",
    "",
    "In the realm of the digital, they've conquered the test,",
    "A hero emerges from the ultimate quest.",
    "Cardy is free, no more in despair,",
    "Thanks to the player's courage, beyond compare.",
    "",
  }
};

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
