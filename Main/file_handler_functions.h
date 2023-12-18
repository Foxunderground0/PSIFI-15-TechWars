#pragma once
#include <LittleFS.h>

inline bool fileExists(const char* path) {
  return LittleFS.exists(path);
}

inline void readPersistedDialogue(long long& story_scene, long long& scene_dialogue_count) {
  File file = LittleFS.open("/dialogue_persist.bin", "r");
  if (file) {
    // Read the values from the file
    String content = file.readStringUntil('\n');

    // Extract values from the content
    sscanf(content.c_str(), "%lld", &story_scene);

    // Move the file cursor to the next line
    content = file.readStringUntil('\n');

    // Extract the second value
    sscanf(content.c_str(), "%lld", &scene_dialogue_count);

    file.close();

  } else {
    Serial.println("Failed to open file for reading");
  }
}


inline void updatePersistedDialogue(long long newStoryScene, long long newSceneDialogueCount) {
  File file = LittleFS.open("/dialogue_persist.bin", "w");
  if (file) {
    // Write the new values to the file with newline character
    file.printf("%lld\n%lld\n", newStoryScene, newSceneDialogueCount);
    file.close();
  } else {
    Serial.println("Failed to open file for writing");
  }
}