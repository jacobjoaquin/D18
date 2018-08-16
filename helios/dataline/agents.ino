
// Default values for all agents
void initAgentList() {
  struct Agent * zPtr = agentList;
  for (int i = 0; i < nAgents; i++) {
    zPtr->position = 0;
    zPtr->length = agentLength;
    zPtr->direction = 0;
    zPtr->color = black;
    zPtr->framesLeft = 0;
    zPtr++;
  }
}

void newAgent() {
  float speed = (random(4) + 1) / 4.0;
  //  float speed = random(2) ? 1 : 0.5;
  int nFrames = random(40, 80);

  Agent * zPtr = &agentList[agentIndex];
  //  zPtr->position = random(nLeds);
  zPtr->position = ledsPerStrip / 2 + (random(nStrips)) * 150;
  zPtr->direction = (random(2) ? 1 : -1) * speed;
  zPtr->framesLeft = nFrames / speed;
  zPtr->nFrames = nFrames / speed;
  zPtr->length = random(1, 8);
  zPtr->color = random(2) == 0 ? pink : orange;

  uint32_t r = random(100);
  if (r < 45) {
    zPtr->color = orange;
  } else if (r < 90) {
    zPtr->color = pink;
  } else {
    zPtr->color = white;
  }

  agentIndex = (agentIndex + 1) % nAgents;
}


void newMirroredAgent() {
  float speed = (random(4) + 1) / 4.0;
  int nFrames = random(40, 80);

  int position = ledsPerStrip / 2 + (random(nStrips)) * 150;
  float direction = (random(2) ? 1 : -1) * speed;
  int length = random(1, 8);
  uint8_t color = random(2) == 0 ? pink : orange;

  createAgent(position, length, direction, color, nFrames);
  createAgent(position, length, -direction, color, nFrames);
}



void createAgent(int position, int length, float direction, uint8_t color, int frames) {
  Agent * zPtr = &agentList[agentIndex];
  zPtr->position = position;
  zPtr->length = length;
  zPtr->direction = direction;
  zPtr->framesLeft = frames;
  zPtr->nFrames = frames;
  zPtr->color = color;

  agentIndex = (agentIndex + 1) % nAgents;
}

//void foo() {
//  //  createAgent(random(nLeds), random(1, 8), 1, pink, agentFrames);
//  int position = random(ledsPerStrip);
//  uint8_t color = random(2) ? pink : orange;
//  int direction = random(2) ? 1 : -1;
//  int length = random(1, 8);
//  if (random(100) < 10) {
//    direction *= 4;
//  }
//  for (int i = 0; i < nStrips; i++) {
//    createAgent(position + i * ledsPerStrip, length, direction, color, agentFrames);
//  }
//}

//void bar() {
//  //  createAgent(random(nLeds), random(1, 8), 1, pink, agentFrames);
//  int position = random(ledsPerStrip);
//  uint32_t color = random(2) ? pink : orange;
//  if (random(100) < 5) {
//    color = 4;
//  }
//  int direction = random(2) ? 1 : -1;
//  int length = random(1, 8);
//
//  //  int offset = length * (random(2) ? 1 : -1);
//  int offset = random(-8, 8);
//
//  if (random(100) < 10) {
//    direction *= 4;
//  }
//  for (int i = 0; i < nStrips; i++) {
//    createAgent(position + i * ledsPerStrip + i * offset, length, direction, color, agentFrames);
//  }
//}

int cycleDisorientIndex = 0;
void cycleDisorient() {
  //  int position = random(ledsPerStrip);
  int position = ledsPerStrip / 2 - 20;
  uint8_t color = random(2) ? pink : orange;
//  uint8_t color = white;
  int direction = random(2) ? 1 : -1;
  int length = random(1, 4);
  length = 1;
  String foo = "disorient  ";
  

  char c = foo[cycleDisorientIndex];
  cycleDisorientIndex = (cycleDisorientIndex + 1) % foo.length();
  uint8_t w = disorientFont2017Widths[(int) c];
  int nFrames = random(60, 100);
  for (int y = 0; y < 8; ++y) {
    int stripOffset = stripOrder[y] * ledsPerStrip;
    
    uint16_t letter = disorientFont2017[c][y];
    for (int x = 0; x < w; x++) {
      int thisX = x;
      if ((letter >> (15 - x)) & 1) {
        createAgent((position + thisX * 4) + stripOffset, length, direction, color, nFrames + random(1, 50));
      }
    }
  }
}


void updateAgents() {
  struct Agent * zPtr = agentList;
  for (int i = 0; i < nAgents; i++) {
    if (zPtr->framesLeft > 0) {
      zPtr->framesLeft--;
      zPtr->position += zPtr->direction;
      uint32_t c = lerpColor(palette[zPtr->color], 0, float(zPtr->framesLeft) / (float) zPtr->nFrames);
      //      uint32_t c = zPtr->color;
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

