#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define LED_COUNT 8
#define BRIGHTNESS 40
#define SPEED_MS   80

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t wheel(uint8_t pos) {
  pos = 255 - pos;
  if (pos < 85) {
    return strip.Color(255 - pos * 3, 0, pos * 3);
  }
  if (pos < 170) {
    pos -= 85;
    return strip.Color(0, pos * 3, 255 - pos * 3);
  }
  pos -= 170;
  return strip.Color(pos * 3, 255 - pos * 3, 0);
}

void showSolid(uint8_t r, uint8_t g, uint8_t b, int waitMs) {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
  delay(waitMs);
}

void colorWipe(uint32_t color, int stepDelayMs) {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(stepDelayMs);
  }
}

void theaterChase(uint32_t color, int stepDelayMs, int cycles) {
  for (int cycle = 0; cycle < cycles; cycle++) {
    for (int offset = 0; offset < 3; offset++) {
      strip.clear();
      for (int i = offset; i < LED_COUNT; i += 3) {
        strip.setPixelColor(i, color);
      }
      strip.show();
      delay(stepDelayMs);
    }
  }
}

void rainbowCycle(int stepDelayMs, int cycles) {
  for (long j = 0; j < 256L * cycles; j++) {
    for (int i = 0; i < LED_COUNT; i++) {
      uint8_t hueIndex = (i * 256 / LED_COUNT + j) & 255;
      strip.setPixelColor(i, wheel(hueIndex));
    }
    strip.show();
    delay(stepDelayMs);
  }
}

void breatheWhite(int stepDelayMs) {
  for (int level = 0; level <= 120; level += 4) {
    strip.fill(strip.Color(level, level, level), 0, LED_COUNT);
    strip.show();
    delay(stepDelayMs);
  }
  for (int level = 120; level >= 0; level -= 4) {
    strip.fill(strip.Color(level, level, level), 0, LED_COUNT);
    strip.show();
    delay(stepDelayMs);
  }
}

void allOff(int waitMs) {
  strip.clear();
  strip.show();
  delay(waitMs);
}

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  allOff(50);
}

void loop() {
  showSolid(255, 0, 0, SPEED_MS * 6);
  showSolid(0, 255, 0, SPEED_MS * 6);
  showSolid(0, 0, 255, SPEED_MS * 6);
  showSolid(255, 255, 255, SPEED_MS * 6);

  colorWipe(strip.Color(255, 100, 0), SPEED_MS);
  colorWipe(strip.Color(0, 0, 0), SPEED_MS / 2);

  theaterChase(strip.Color(0, 180, 180), SPEED_MS, 12);
  rainbowCycle(SPEED_MS / 2, 2);
  breatheWhite(SPEED_MS / 2);

  allOff(SPEED_MS * 3);
}
