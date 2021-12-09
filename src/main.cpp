#include <Adafruit_WS2801.h>

/*  RST       VCC
     3         2
     4         1 
    GND        0

*/

#define LED_CLK 4
#define LED_DATA 3
#define BTN 0
#define NUM_LEDS 9

#define DEBOUNCE_THRESHOLD 2

#define MODE_RED 0
#define MODE_GREEN 1
#define MODE_BLUE 2
#define MODE_PINK 3
#define MODE_WARM_WHITE 4
#define MODE_RAINBOW 5
#define MODE_RAINBOW_CYCLE 6
#define MODE_OFF 7

Adafruit_WS2801 leds = Adafruit_WS2801(10, LED_DATA, LED_CLK);
uint8_t mode = MODE_OFF;
uint8_t debounce;
uint8_t rainbowOffset = 0;

uint32_t RGB(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t c = r;
    c <<= 8;
    c |= g;
    c <<= 8;
    c |= b;
    return c;
}

uint32_t HSV(uint8_t hue) {
    if (hue < 85) 
        return RGB(hue * 3, 255 - hue * 3, 0);
    hue -= 85;
    if (hue < 85)
       return RGB(255 - hue * 3, 0, hue * 3);
    hue -= 85;
    return RGB(0, hue * 3, 255 - hue * 3);
}

void fill(uint32_t color) {
    for (int i = 0; i < NUM_LEDS; ++i)
        leds.setPixelColor(i, color);
}

void rainbowFill(uint8_t offset) {
    uint8_t delta = 256 / NUM_LEDS;
    for (int i = 0; i < NUM_LEDS; ++i, offset += delta)
        leds.setPixelColor(i, HSV(offset));
}

void nextMode() {
    mode = (mode + 1) % 8;
    switch (mode) {
        case MODE_RED:
            fill(RGB(255, 0, 0));
            break;
        case MODE_GREEN:
            fill(RGB(0, 255, 0));
            break;
        case MODE_BLUE:
            fill(RGB(0, 0, 255));
            break;
        case MODE_PINK:
            fill(RGB(255, 53, 184));
            break;
        case MODE_WARM_WHITE:
            fill(RGB(255, 211, 64));
            break;
        case MODE_RAINBOW:
        case MODE_RAINBOW_CYCLE:
            rainbowFill(0);
            break;
        case MODE_OFF:
            fill(RGB(0,0,0));
            break;
    }
    leds.show();
}

void setup() {
    pinMode(BTN, INPUT_PULLUP);
    leds.begin();
    leds.show();
    nextMode();
}

void loop() {
  if (digitalRead(BTN) == LOW) {
      if (debounce > 0 && --debounce == 0) 
          nextMode();
  } else {
      debounce = DEBOUNCE_THRESHOLD;
  }
  delay(10);
  switch (mode) {
      case MODE_RAINBOW_CYCLE:
          ++rainbowOffset;
          rainbowFill(rainbowOffset);
          leds.show();
          break;

  }
}
