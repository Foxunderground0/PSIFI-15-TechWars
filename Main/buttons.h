#pragma once

extern const int button_pin_a;
extern const int button_pin_b;
extern const int button_pin_c;
extern const int button_pin_d;
extern const int button_pin_e;

extern volatile bool state_a;
extern volatile bool state_b;
extern volatile bool state_c;
extern volatile bool state_d;
extern volatile bool state_e;

extern unsigned long lastPressTime_a;
extern unsigned long lastPressTime_b;
extern unsigned long lastPressTime_c;
extern unsigned long lastPressTime_d;
extern unsigned long lastPressTime_e;

extern const unsigned long debounceThreshold;

// Non-blocking Debounce Interrupt Service Routines
inline void IRAM_ATTR buttonA_ISR() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastPressTime_a >= debounceThreshold) {
    lastPressTime_a = currentMillis;
    state_a = digitalRead(button_pin_a);
  }
}

inline void IRAM_ATTR buttonB_ISR() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastPressTime_b >= debounceThreshold) {
    lastPressTime_b = currentMillis;
    state_b = digitalRead(button_pin_b);
  }
}

inline void IRAM_ATTR buttonC_ISR() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastPressTime_c >= debounceThreshold) {
    lastPressTime_c = currentMillis;
    state_c = digitalRead(button_pin_c);
  }
}

inline void IRAM_ATTR buttonD_ISR() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastPressTime_d >= debounceThreshold) {
    lastPressTime_d = currentMillis;
    state_d = digitalRead(button_pin_d);
  }
}

inline void IRAM_ATTR buttonE_ISR() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastPressTime_e >= debounceThreshold) {
    lastPressTime_e = currentMillis;
    state_e = digitalRead(button_pin_e);
  }
}
