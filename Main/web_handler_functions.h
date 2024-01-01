#pragma once
#include <LittleFS.h>
#include "file_handler_functions.h"
#include "utils.h"

void handleRoot(ESP8266WebServer& server, bool& dialogReady, bool& isGame) {
  if (isGame) {
    const char* gzFilePath = "/game.html.gz"; // Adjust the path as needed

    // Open the compressed file from LittleFS
    File gzFile = LittleFS.open(gzFilePath, "r");
    if (!gzFile) {
      server.send(500, "text/plain", "Failed to open compressed file for reading");
      return;
    }

    // Send the gzipped file content
    server.streamFile(gzFile, "text/html");

    // Close the file
    gzFile.close();
  } else {
    const char* gzFilePath = "/index.html.gz"; // Adjust the path as needed

    // Open the compressed file from LittleFS
    File gzFile = LittleFS.open(gzFilePath, "r");
    if (!gzFile) {
      server.send(500, "text/plain", "Failed to open compressed file for reading");
      return;
    }

    // -- Apparently it automatically detects that the encoding is .gz so we dont explicitly need to define this --
    // Set the Content-Encoding header to indicate gzip compression
    //server.sendHeader("Content-Encoding", "gzip");

    // Send the gzipped file content
    server.streamFile(gzFile, "text/html");

    // Close the file
    gzFile.close();

    // Set dialogReady to true after sending the response
    dialogReady = true;
  }

}

inline void handleMKV(ESP8266WebServer& server) {
  const char* filePath = "/output_final.mkv"; // Adjust the path as needed

  // Open the file from LittleFS
  File file = LittleFS.open(filePath, "r");
  if (!file) {
    server.send(500, "text/plain", "Failed to open file for reading");
    return;
  }

  // Get the filename from the path
  String filename = String(filePath);
  int lastSlashIndex = filename.lastIndexOf('/');
  if (lastSlashIndex != -1) {
    filename = filename.substring(lastSlashIndex + 1);
  }

  // Set the Content-Disposition header to suggest a filename
  server.sendHeader("Content-Disposition", "inline; filename=" + filename);

  // Send the file content
  server.streamFile(file, "video/x-matroska");

  // Close the file
  file.close();
}

inline void handleBootTime(ESP8266WebServer& server) {
  // Respond with the current value of millis
  server.send(200, "text/plain", String(millis()) + "." + String(micros()).substring(String(micros()).length() - 3));
}

inline void handleFSContent(ESP8266WebServer& server, const String& dialogue_file_path) {
  File file = LittleFS.open(dialogue_file_path, "r");
  if (file) {
    // Read the content of the file
    String fileContent = file.readString();
    file.close();

    // Send the file content as the response
    server.send(200, "text/plain", fileContent);
  } else {
    // If the file cannot be opened, send an error response
    server.send(500, "text/plain", "Failed to open file for reading");
  }
}

inline void handleDialogReady(ESP8266WebServer& server, bool& dialogReady, bool& scene_dialogue_completed) {
  server.send(200, "text/plain", (dialogReady ? "1" : "0"));
  scene_dialogue_completed = !dialogReady;  // Update the scene_dialogue_completed based on dialogReady
  dialogReady = false;
}

void handlePastDialogue(ESP8266WebServer& server, const String(&dialogues)[][20], bool& scene_dialogue_completed, long long& story_scene, long long& scene_dialogue_count) {
  String response = "";

  // Concatenate strings from dialogues[story_scene][0] to dialogues[story_scene][scene_dialogue_count-1]
  for (long long i = 0; i < scene_dialogue_count; ++i) {
    response += dialogues[story_scene][i];

    // You can add a separator between dialogues if needed, for example, a newline character
    response += "\n";
  }

  server.send(200, "text/plain", response);
  scene_dialogue_completed = true;
}

inline void handleLatestDialogue(ESP8266WebServer& server, const String(&dialogues)[][20], const int& buzzer_pin, long long& story_scene, long long& scene_dialogue_count, int dialogues_count[], bool& dialogReady, bool& scene_dialogue_completed, bool& scan_for_rssi) {
  scene_dialogue_completed = false;
  String response = dialogues[story_scene][scene_dialogue_count];
  long long num_of_characters = response.length();
  server.send(200, "text/plain", response);

  //Create the buzzer chirping sound
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;  // Initialize elapsed time to zero

  while (elapsedTime < num_of_characters * 65) {
    unsigned long currentTime = millis();
    elapsedTime = currentTime - startTime;

    int randomFrequency = random(300, 2000);  // Random frequency between 300Hz and 1000Hz
    int randomDelay = random(10, 200);        // Random delay between 10ms and 500ms
    int randomToneTime = random(10, 200);     // Random delay between 10ms and 500ms

    if (elapsedTime + randomDelay < num_of_characters * 65) {
      // Play a tone with the random frequency
      tone(buzzer_pin, randomFrequency, randomToneTime);  // Play the tone for 50ms
      delay(randomDelay);                                 // Add the random delay
    }
  }

  // Done chirping
  noTone(buzzer_pin);

  // Done chirping
  digitalWrite(buzzer_pin, LOW);

  scene_dialogue_count++;

  if (scene_dialogue_count == dialogues_count[story_scene]) {
    scene_dialogue_count = 0;
    story_scene++;
    dialogReady = false;
    scene_dialogue_completed = true;

    if (story_scene == 1) {
      scan_for_rssi = true;
    }

  } else {
    dialogReady = true;
  }

  updatePersistedDialogue(story_scene, scene_dialogue_count);
}

inline void handleCMD(ESP8266WebServer& server, const String& teamName, const int& buzzer_pin) {
  String command = server.arg("command");
  SerialPrintLn(command);
  String response = "";

  if (command == "help") {
    response = "stats : Gives System Info";
  } else if (command == "stats") {
    response =
      "<br><div style=\"white-space: pre;\"><span class=\"red\">                                ,(###########                    </span>  | <span class=\"blue\">System Uptime:</span> " + String(millis()) + "." + String(micros()).substring(String(micros()).length() - 3) + " ms <br>" + "<span class=\"red\">                    ######(##         ############               </span>  | <span class=\"blue\">User:</span> " + teamName + "<br>" + "<span class=\"red\">          ###    ##################(      *(#########            </span>  | <span class=\"blue\">IP:</span> " + String(WiFi.localIP().toString().c_str()) + "<br>" + "<span class=\"red\">        ###      ######################(      #########,         </span>  | <span class=\"blue\">Resolution:</span> 130x100 <br>" + "<span class=\"red\">       ##.              #(#################(     ########        </span>  | <span class=\"blue\">CPU:</span> Tensilica L106 <br>" + "<span class=\"red\">     ,##                       .##############.    (######(      </span>  | <span class=\"blue\">Architecture:</span> 32-bit RISC <br>" + "<span class=\"red\">     ##     #################       ############(    (######     </span>  | <span class=\"blue\">Frequency:</span> 160 MHz <br>" + "<span class=\"red\">    (#   ,######################(#     ###########(    ######    </span>  | <span class=\"blue\">Flash:</span> " + String(ESP.getFreeSketchSpace()) + " / 4,194,304 <br>" + "<span class=\"red\">   ###  #############################     ###########   .####*   </span>  | <span class=\"blue\">Free Heap:</span> " + String(ESP.getFreeHeap()) + "<br>" + "<span class=\"red\">   ##   ########,        ##############/    #########(    ####   </span>  | <span class=\"blue\">Kernel:</span>  Fox-OS <br>" + "<span class=\"red\">   ##   ##########(#(        .(##########(    #########    ###   </span>  | <br>" + "<span class=\"red\">   ##    ##################      #########(    #########    #    </span>  | <br>" + "<span class=\"red\">   ###     ###################     #########*   ########(        </span>  | <br>" + "<span class=\"red\">    ##        ,#################    (########/   #########       </span>  | <br>" + "<span class=\"red\">    .##                ###########   (########    ########,      </span>  | <br>" + "<span class=\"red\">     *##                 ##########   #########   #########      </span>  | <br>" + "<span class=\"red\">       #(    .#######,    /########    ########   .#####(.       </span>  | <br>" + "<span class=\"red\">        (#(  #########     ########    ########    ####/         </span>  | <br>" + "<span class=\"red\">          ##( #######     (########    #######(                  </span>  | <br>" + "<span class=\"red\">            ####         #########(   #########       (##        </span>  | <br>" + "<span class=\"red\">               (###       ########    #######      ####          </span>  | <br>" + "<span class=\"red\">                  .#####                      #####              </span>  | <br>" + "<span class=\"red\">                        (#######(###########(                    </span>  | </div>";
  } else {
    response = "-bash: " + command + ": command not found";
  }

  server.send(200, "text/plain", response);

  digitalWrite(buzzer_pin, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(50);                       // wait for a second
  digitalWrite(buzzer_pin, LOW);   // turn the LED off by making the voltage LOW
}

inline void handleRawData(ESP8266WebServer& server, const String& rawData) {
  // Respond with the current value of rawData
  server.send(200, "text/plain", rawData);
}
