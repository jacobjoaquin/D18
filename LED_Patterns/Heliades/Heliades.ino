/*
  Heliades.ino - Heliades Pattern
  Inspired by Helios, Disorient 2014, by Leo Villareal.
  Coded by Jacob Joaquin.
*/

#include <Adafruit_NeoPixel.h>

// Combines red, green, blue into a single value
#define rgb(R, G, B)  ((((uint32_t)(R)) << 16) | (((uint32_t)(G)) << 8) | ((uint32_t)(B)))

const int PIN = 13;
const int nLeds = 60;
const int framesPerSecond = 30;
const int frameDelay = 1000 / framesPerSecond;


Adafruit_NeoPixel leds(nLeds, PIN, NEO_GRB + NEO_KHZ800);;

// Heliades
bool heliades[nLeds];
bool heliadesReverse[nLeds];
int offset = 0;
int forwardOdds = 128;  // Chance on will flip to off. [0-255]
int reverseOdds = 32;   // Chance off will flip to on. [0-255]

void setup() {
  leds.begin();
  createHeliadesBuffer(heliades);
  createHeliadesBuffer(heliadesReverse);
}

void loop() {
  leds.clear();
  for (int i = 0; i < nLeds; i++) {
    int r = nLeds - i - 1;
    int forwardOffset = (i + offset) % nLeds;
    int reverseOffset = (r + offset) % nLeds;
    bool forward = heliades[forwardOffset];
    bool reverse = heliadesReverse[reverseOffset];

    uint32_t color = 0;
    
    if (forward) {
      int b = random(16, 64);
      if (random(100) < 5) {
        b = 255;
      }
      color = rgb(b, b, b);
      leds.setPixelColor(i, color);
    }
    if (reverse) {
      int b = random(16, 64);
      if (random(100) < 5) {
        b = 255;
      }
      color = rgb(b, b, b);
      leds.setPixelColor(i, color);
    }
  }

  offset = (offset + 1) % nLeds;
  displayLEDs();
}

uint32_t showTime = millis() + frameDelay;
void displayLEDs() {
  while (millis() < showTime) {}
  leds.show();
  showTime = millis() + frameDelay;
}

void createHeliadesBuffer(bool *buffer) {
  bool flip = true;
  for (int i = 0; i < nLeds; i++) {
    buffer[i] = flip;

    if (flip) {
      if (random(255) < forwardOdds) {
        flip = !flip;
      }
    } else {
      if (random(255) < reverseOdds) {
        flip = !flip;
      }
    }
  }
}
