#include <Adafruit_NeoPixel.h>

#define PIN 13
#define N_PIXELS 60
Adafruit_NeoPixel strip;
uint8_t theBuffer[N_PIXELS];

// User defined settings
uint32_t headColor = strip.Color(255, 255, 255);
uint32_t tailColor = strip.Color(64, 64, 64);
uint32_t endColor = 0;
int nSparklesPerFrame = 1;
const int framesPerSecond = 60;

const int headLength = 2;
const int tailLength = 10;
const int gradientLength = headLength + tailLength;
uint32_t gradient[gradientLength];

// Timing
const int frameDelay = 1000 / framesPerSecond;
uint32_t showTime = millis() + frameDelay;


void setup() {
  strip = Adafruit_NeoPixel(N_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
  createGradient();
  for (int i = 0; i < N_PIXELS; i++) {
    theBuffer[i] = 0;
  }
  strip.begin();
  strip.clear();
  strip.show();
}

void createGradient() {
  for (int i = 0; i < headLength; i++) {
    gradient[i] = lerpColor(headColor, tailColor, (float(i) / float(headLength)));
  }
  for (int i = headLength; i < headLength + tailLength; i++) {
    gradient[i] = lerpColor(tailColor, endColor, (float(i - headLength) / float(tailLength)));
  }
  gradient[gradientLength - 1] = 0;
}

void loop() {
  showTime = millis() + frameDelay;
  
  for (int i = 0; i < nSparklesPerFrame; ++i) {
    int r = random(N_PIXELS);
    theBuffer[r] = gradientLength;
  }
  doSparkle();
  while (millis() < showTime) {}
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

void doSparkle() {

  for (int i = 0; i < N_PIXELS; i++) {
    uint8_t b = max(theBuffer[i] - 1, 0);
    theBuffer[i] = b;
    strip.setPixelColor(i, gradient[gradientLength - 1 - theBuffer[i]]);
  }

  strip.show();
}

