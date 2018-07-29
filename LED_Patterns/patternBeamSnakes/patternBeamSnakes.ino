/*
patternBeamSnakes.ino - Beam Snakes Pattern
Coded by Jacob Joaquin.
*/

#include <Adafruit_NeoPixel.h>

// Combine rbg values to color
#define rgb(R, G, B)  ((((uint32_t)(R)) << 16) | (((uint32_t)(G)) << 8) | ((uint32_t)(B)))

// Constansts
const int PIN = 13;
const int nLeds = 60;
const int frameDelay = 1000 / 60;


Adafruit_NeoPixel leds(nLeds, PIN, NEO_GRB + NEO_KHZ800);;

// User defined variables
uint32_t orange = rgb(128, 24, 0);
uint32_t magenta = rgb(192, 0, 92);
uint32_t black = rgb(0, 0, 0);
uint32_t white = rgb(255, 255, 255);
int pos = 0;

// Set tail properties
uint32_t tailColor = orange;
uint32_t headColor = magenta;
int tail = 12;
int head = 4;

void setup() {
  leds.begin();
}

void loop() {
  clear();

  for (int i = 0; i < tail; i++) {
    uint32_t thisColor;

    // Head
    if (i > tail - head) {
      thisColor = lerpColor(tailColor, headColor, (float) (i - (tail - head)) / (float) head);
    }
    // Tail
    else {
      thisColor = lerpColor(black, tailColor, (float) i / (float) (tail - head));
    }

    int forwardIndex = (i + pos) % nLeds;
    int reverseIndex = nLeds - 1 - forwardIndex;

//    for (int j = 0; j < nBeams; j++) {
//      int beamOffset = j;
      leds.setPixelColor(forwardIndex, thisColor);
      leds.setPixelColor(reverseIndex, thisColor);
//    }
  }
  pos = (pos + 1) % nLeds;

  displayLEDs();
}

uint32_t showTime = millis() + frameDelay;
void displayLEDs() {
  while(millis() < showTime) {}
  leds.show();
  showTime = millis() + frameDelay;
}

// Clear all the pixels
void clear() {
  for (int i = 0; i < nLeds; i++) {
    leds.setPixelColor(i, 0);
  }
}

// Interpolate between two colors. amt: [0.0-1.0)
uint32_t lerpColor(uint32_t c1, uint32_t c2, float amt) {
  int i = (int) (amt * 256.0) + 1;
  int di = 256 - i;
  uint32_t r1 = (c1 & 0xff0000) >> 16;
  uint32_t g1 = (c1 & 0x00ff00) >> 8;
  uint32_t b1 = (c1 & 0x0000ff);
  uint32_t r2 = (c2 & 0xff0000) >> 16;
  uint32_t g2 = (c2 & 0x00ff00) >> 8;
  uint32_t b2 = (c2 & 0x0000ff);
  r1 = ((r2 * i) + (r1 * di)) >> 8;
  g1 = ((g2 * i) + (g1 * di)) >> 8;
  b1 = ((b2 * i) + (b1 * di)) >> 8;
  return (r1 << 16) | (g1 << 8) | b1;
}

