#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 13
#define N_PIXELS 60

Adafruit_NeoPixel strip;
uint32_t pattern[N_PIXELS];
int position = 0;

uint32_t orange = strip.Color(128, 24, 0);
uint32_t pink = strip.Color(128, 0, 64);
uint32_t white = strip.Color(255, 255, 255);

// User defined settings
const int framesPerSecond = 60;
uint32_t headColor = pink;
uint32_t tailColor = orange;
uint32_t endColor = 0;
int headLength = 5;
int tailLength = 13;

// Timing
const int frameDelay = 1000 / framesPerSecond;
uint32_t showTime = millis() + frameDelay;


void setup() {
  strip = Adafruit_NeoPixel(N_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

  pattern[0] = lerpColor(white, headColor, 0.75);
  for (int i = 1; i < headLength; i++) {
    pattern[i] = lerpColor(headColor, tailColor, (float(i) / float(headLength)));
  }
  for (int i = headLength; i < headLength + tailLength; i++) {
    pattern[i] = lerpColor(tailColor, endColor, (float(i - headLength) / float(tailLength)));
  }

  strip.begin();
  strip.show();
}

void loop() {
  while (millis() < showTime) {}
  cycleBuffer();
  showTime = millis() + frameDelay;
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

void cycleBuffer() {
  for (int i = 0; i < N_PIXELS; i++) {
    int thisPosition = (i + position) % N_PIXELS;    

    strip.setPixelColor(i, pattern[thisPosition]);
  }
  
  position = ++position % N_PIXELS;  
  strip.show();
}

