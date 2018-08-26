bool isGlitching = false;
uint32_t nextGlitch = random(100);
int glitchFrames = random(1, 6);
int glitchStrip = random(100);

void resetGlitch() {
  nextGlitch = frame + random(1, 60);
  glitchFrames = random(1, 10);
  isGlitching = false;
  glitchStrip = random(nStrips);
}

// Random strip flicker
void glitch() {
  if (frame >= nextGlitch) {
    isGlitching = true;
  }

  if (isGlitching) {
    if (frame >= nextGlitch && frame < nextGlitch + glitchFrames) {
      int start = ledsPerStrip * glitchStrip;
      int nFlicker = ledsPerStrip;
      for (int i = start; i < start + nFlicker; ++i) {
        int index = i % nLeds;
        if (buffer[index] >= 1) {
          if (random(100) < 60) {
            buffer[index] = palette[white];
          } else {
            buffer[index] = palette[yellow];
          }
        }
      }
    } else {
      if (frame >= nextGlitch + glitchFrames) {
        resetGlitch();
      }
    }
  }
}
//  if (!(frame % 12)) {
//    int start = ledsPerStrip * random(nStrips);
//    int nFlicker = ledsPerStrip;
//    for (int i = start; i < start + nFlicker; ++i) {
//      int index = i % nLeds;
//      if (buffer[index] >= 1) {
//        if (random(100) < 60) {
//          buffer[index] = palette[white];
//        } else {
//          buffer[index] = palette[cyan];
//        }
//      }
//    }
//  }

