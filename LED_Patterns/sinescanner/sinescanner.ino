#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

const int PIN = 13;
const int N_PIXELS = 60;
const int SCANNER_LENGTH = 11;

// Internal variables
Adafruit_NeoPixel strip(N_PIXELS, PIN, NEO_GRB + NEO_KHZ800);;
uint32_t theBuffer[SCANNER_LENGTH + 1];  // Extra guard point for interpolation
float phase = 0.5;
int direction = 1;

// User defined settings
float phaseInc = 0.003;


void setBufferColor(uint32_t c1, uint32_t c2, uint8_t offset, uint8_t theLength) {
  for (int i = 0; i < theLength; i++) {
    theBuffer[i + offset] = lerpColor(c1, c2, float(i) / float(theLength));
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

void loadBuffer() {
//  setBufferColor(0, 0xff8800, 0, 5);
//  theBuffer[5] = 0xff9922;
//  setBufferColor(0xff8800, 0, 6, 5);
//  theBuffer[SCANNER_LENGTH] = 0;
  setBufferColor(0, 0xff0011, 0, 5);
  theBuffer[5] = 0xff0033;
  setBufferColor(0xff0011, 0, 6, 5);
  theBuffer[SCANNER_LENGTH] = 0;
}

void updateScanner() {
  strip.clear();

  float p = (sin(phase * TWO_PI) + 1.0) / 2.0 * N_PIXELS + 0.5;
  int p0 = p;
  float interp = 1 - (p - float(p0));
  int offset = SCANNER_LENGTH / 2;
  
  for (int i = 0; i < SCANNER_LENGTH; i++) {
    int pos = p0 - offset + i;
    uint32_t c = lerpColor(theBuffer[i], theBuffer[i + 1], interp);
    
    if (pos >= 0 && pos < N_PIXELS) {
      strip.setPixelColor(pos, c);
    }
  }

  strip.show();
  phase += phaseInc;
  phase -= phase >= 1.0 ? 1 : 0;
}

void setup() {
  strip.begin();
  strip.show();
  loadBuffer();
}

void loop() {
  uint32_t waitTime = 1000 / 120;
  uint32_t t = millis() + waitTime;
  updateScanner();
  strip.show();
  while (millis() < t) { }
//  delay(5);
}
