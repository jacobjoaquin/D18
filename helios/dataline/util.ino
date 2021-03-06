// New random seed based on analogRead0 and analogRead1
void setupRandomSeed() {
  uint32_t r0 = analogRead(0) + analogRead(1) + 1;
  uint32_t r1 = 0;
  for (uint32_t i = 0; i < r0; i++) {
    r1 += analogRead(0) + analogRead(1) + analogRead(2);
  }
  randomSeed(r1);
}


// Display the current LED Buffer
void displayLEDs() {
  if (millis() > sanityNextSwitch) {
    sanityLED = !sanityLED;
    digitalWrite(sanityPin, sanityLED);
    sanityNextSwitch = millis() + sanityDelay;
  }
  while (millis() < showTime) {}
  leds.show();
  showTime = millis() + frameDelay;
}

// Clear all the pixels
void clear() {
  for (int i = 0; i < nLeds; i++) {
    leds.setPixel(i, 0);
  }
}

// Buffer to LEDs
void bufferToLEDs() {
  uint32_t * bufferPtr = buffer;

  for (int i = 0; i < nLeds; i++) {
    uint32_t c = *bufferPtr;
    uint8_t r = randomBuffer[i];
    c = lerpColor(0, c, r);
//    leds.setPixel(i, *bufferPtr);
    leds.setPixel(i, c);
    ++bufferPtr;
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

// Interpolate between two colors. amt: [0-255]
uint32_t lerpColor(uint32_t c1, uint32_t c2, int amt) {
  int i = amt + 1;
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

uint32_t shiftColor(uint32_t c, int shift) {
  uint32_t r1 = (c & 0xff0000) >> 16;
  uint32_t g1 = (c & 0x00ff00) >> 8;
  uint32_t b1 = (c & 0x0000ff);
  r1 = r1 >> shift;
  g1 = g1 >> shift;
  b1 = b1 >> shift;
  return (r1 << 16) | (g1 << 8) | b1;
}
