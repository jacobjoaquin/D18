#include <Adafruit_NeoPixel.h>

// Combine rbg values to color
#define rgb(R, G, B)  ((((uint32_t)(R)) << 16) | (((uint32_t)(G)) << 8) | ((uint32_t)(B)))

// Constansts
const int PIN = 13;
const int nLeds = 60;
const int frameDelay = 1000 / 12;
const int nSparklesPerFrame = 1;

// Neopixel setup
Adafruit_NeoPixel leds(nLeds, PIN, NEO_GRB + NEO_KHZ800);;


// User defined variables
uint32_t orange = rgb(255, 64, 0);
uint32_t magenta = rgb(255, 0, 92);
uint32_t black = rgb(0, 0, 0);
uint32_t white = rgb(255, 255, 255);
uint32_t pink = rgb(128, 0, 64);

// LED Buffer
uint32_t buffer[nLeds] = {0};

// Timing
uint32_t showTime = millis() + frameDelay;

// Sparkle
struct Sparkle {
  int position;
  int length;
  int direction;
  uint32_t color;
  int framesLeft;
};

const int nSparkles = 100;
Sparkle sparkleList[nSparkles];
int sparkleLength = 5;
int sparkleFrames = 40;
int sparkleIndex = 0;

void initSparkleList() {
  struct Sparkle * zPtr = sparkleList;
  for (int i = 0; i < nSparkles; i++) {
    zPtr->position = 0;
    zPtr->length = sparkleLength;
    zPtr->direction = 0;
    zPtr->color = pink;
    zPtr->framesLeft = 0;
    zPtr++;
  }
}

void newSparkle() {
  Sparkle * zPtr = &sparkleList[sparkleIndex];
  zPtr->position = random(nLeds);
  zPtr->direction = random(2) ? 1 : -1;
  zPtr->framesLeft = sparkleFrames;
  zPtr->length = random(1, 5);

  // Set the color
  uint32_t r = random(100);
  if (r < 49) {
    zPtr->color = pink;
  } else if (r < 98) {
    zPtr->color = orange;
  } else {
    zPtr->color = white;
  }

  // Update index
  sparkleIndex = (sparkleIndex + 1) % nSparkles;
}

void updateSparkles() {
  struct Sparkle * zPtr = sparkleList;
  for (int i = 0; i < nSparkles; i++) {
    if (zPtr->framesLeft > 0) {
      zPtr->framesLeft--;
      zPtr->position += zPtr->direction;
      uint32_t c = lerpColor(zPtr->color, 0, float(zPtr->framesLeft) / (float) sparkleFrames);
      int length = zPtr->length;
      int position = zPtr->position;

      for (int j = 0; j < length; j++) {
        int index = position + j;
        index += nLeds * (index < 0);
        index -= nLeds * (index >= nLeds);
        buffer[index] = c;
      }
    }
    zPtr++;
  }
}

void bufferToLEDs() {
  uint32_t * bufferPtr = buffer;

  // Add additional sparkle
  for (int i = 0; i < nLeds; i++) {
    uint32_t c = *bufferPtr;
    int amt = random(256);
    c = lerpColor(0, c, amt);
    leds.setPixelColor(i, c);
    bufferPtr++;
  }
}

void setup() {
  initSparkleList();
  leds.begin();
}

void loop() {
  memset(&buffer[0], 0, sizeof(buffer));
  leds.clear();
  for (int i = 0; i < nSparklesPerFrame; ++i) {
    newSparkle();

  }
  updateSparkles();
  bufferToLEDs();
  displayLEDs();
}

void displayLEDs() {
  while (millis() < showTime) {}
  leds.show();
  showTime = millis() + frameDelay;
}
