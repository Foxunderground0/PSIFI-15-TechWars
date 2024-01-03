#pragma once

#include <MD_MAX72xx.h>
#include "buttons.h"

// Variables
extern volatile bool is_scrolling;
extern uint8_t scroll_state;
extern const char* scrolling_text;
extern char* scroll_text_ptr;
extern uint8_t current_char_width;
extern uint8_t char_buffer[8];
extern uint8_t sub_char_scroll_count;
extern MD_MAX72XX mx;
extern const unsigned long TEXTSCROLLDELAY;

void scrollTextNonBlocking() {
  handleAllButtons();
  if (is_scrolling) {
    if (scroll_state == 0) {
      if (*scroll_text_ptr != '\0') {
        current_char_width = mx.getChar(*scroll_text_ptr++, sizeof(char_buffer) / sizeof(char_buffer[0]), char_buffer);
        scroll_state = 1;
      } else {
        scroll_text_ptr = (char*)scrolling_text;
      }
    }

    if (scroll_state == 1) {
      if (sub_char_scroll_count <= current_char_width)  // allow space between characters
      {
        mx.transform(MD_MAX72XX::TSL);
        mx.setColumn(0, (sub_char_scroll_count < current_char_width) ? char_buffer[sub_char_scroll_count] : 0);
        sub_char_scroll_count++;
        //delay(TEXTSCROLLDELAY);
      }

      if (sub_char_scroll_count == current_char_width) {
        mx.transform(MD_MAX72XX::TSL);
        scroll_state = 0;
        sub_char_scroll_count = 0;
      }
    }
  }
}

void scrollTextBlocking(const char* p) {
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
