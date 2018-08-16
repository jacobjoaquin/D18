/*
  shimmer.ino
  Coded by Jacob Joaquin.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

  pin 2:  LED Strip #0
  pin 14: LED strip #1
  pin 7:  LED strip #2
  pin 8:  LED strip #3
  pin 6:  LED strip #4
  pin 20: LED strip #5
  pin 21: LED strip #6
  pin 5:  LED strip #7
*/


/*
  TODO:
  random seed
  shapes:
  Chevron
  slashes
  bars
  letters
  Try stationary agents
  Color Mixing?
  Black Pixels
  Faster or slower pixels?
  Random frame length
*/


#include <OctoWS2811.h>
#include "disfont2017.h"

// Combine rbg values to color
#define rgb(R, G, B)  ((((uint32_t)(R)) << 16) | (((uint32_t)(G)) << 8) | ((uint32_t)(B)))

// Constansts
const int ledsPerStrip = 150;
const int nStrips = 8;
const int nLeds = ledsPerStrip * nStrips;
const int frameRate = 48;
const int frameDelay = 1000 / frameRate;

// Octows2811 Setup
DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// User defined variables
uint32_t orange = rgb(255, 64, 0);
uint32_t magenta = rgb(255, 0, 92);
uint32_t black = rgb(0, 0, 0);
uint32_t white = rgb(255, 255, 255);
uint32_t pink = rgb(128, 0, 64);


// LED Buffer
uint32_t buffer[nLeds] = {0};

// Sanity led
boolean sanityLED = true;
uint32_t sanityDelay = 48;
uint32_t sanityNextSwitch;
int sanityPin = 13;

// Agent
struct Agent {
  float position;
  int length;
  int direction;
  uint32_t color;
  int framesLeft;
};

const int nAgents = 1600;
Agent agentList[nAgents];
int agentLength = 5;
int agentFrames = 40;
int agentIndex = 0;

// Timing
ulong showTime = millis() + frameDelay;
uint32_t frame = 0;

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
  Agent * zPtr = &agentList[agentIndex];
  zPtr->position = random(nLeds);
  zPtr->direction = random(2) ? 1 : -1;
  zPtr->framesLeft = agentFrames;
  zPtr->length = random(1, 8);
  //  zPtr->color = white;
  zPtr->color = random(2) == 0 ? pink : orange;

  uint32_t r = random(100);
  if (r < 45) {
    zPtr->color = pink;
  } else if (r < 90) {
    zPtr->color = orange;
  } else {
    zPtr->color = white;
  }

  agentIndex = (agentIndex + 1) % nAgents;
}

void createAgent(int position, int length, int direction, uint32_t color, int frames) {
  //    int position;
  //  int length;
  //  int direction;
  //  uint32_t color;
  //  int framesLeft;

  Agent * zPtr = &agentList[agentIndex];
  zPtr->position = position;
  zPtr->length = length;
  zPtr->direction = direction;
  zPtr->framesLeft = frames;
  zPtr->color = color;

  agentIndex = (agentIndex + 1) % nAgents;
}

void foo() {
  //  createAgent(random(nLeds), random(1, 8), 1, pink, agentFrames);
  int position = random(ledsPerStrip);
  uint32_t color = random(2) ? pink : orange;
  int direction = random(2) ? 1 : -1;
  int length = random(1, 8);
  if (random(100) < 10) {
    direction *= 4;
  }
  for (int i = 0; i < nStrips; i++) {
    createAgent(position + i * ledsPerStrip, length, direction, color, agentFrames);
  }
}

void bar() {
  //  createAgent(random(nLeds), random(1, 8), 1, pink, agentFrames);
  int position = random(ledsPerStrip);
  uint32_t color = random(2) ? pink : orange;
  if (random(100) < 5) {
    color = white;
  }
  int direction = random(2) ? 1 : -1;
  int length = random(1, 8);

  //  int offset = length * (random(2) ? 1 : -1);
  int offset = random(-8, 8);

  if (random(100) < 10) {
    direction *= 4;
  }
  for (int i = 0; i < nStrips; i++) {
    createAgent(position + i * ledsPerStrip + i * offset, length, direction, color, agentFrames);
  }
}

void baz() {
  int position = random(ledsPerStrip);
  uint32_t color = random(2) ? pink : orange;
    int direction = random(2) ? 1 : -1;
  int length = random(1, 3);
  String foo = "disorient";
  
  char c = foo[random(9)];
  uint8_t w = disorientFont2017Widths[(int) c];
  for (int y = 0; y < 8; ++y) {
    uint16_t letter = disorientFont2017[c][y];
    for (int x = 0; x < w; x++) {
      int thisX = x;
      if ((letter >> (15 - x)) & 1) {
        createAgent((position + thisX * 3) + y * ledsPerStrip, length, direction, color, agentFrames);
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
      uint32_t c = lerpColor(zPtr->color, 0, float(zPtr->framesLeft) / (float) agentFrames);
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

  // Add additional agent
  for (int i = 0; i < nLeds; i++) {
    uint32_t c = *bufferPtr;
    int amt = random(256);
    c = lerpColor(0, c, amt);
    leds.setPixel(i, c);
    bufferPtr++;
  }
}

void setupRandomSeed() {
  auto r0 = analogRead(0) + analogRead(1) + 1;
  uint32_t r1 = 0;
  for (uint32_t i = 0; i < r0; i++) {
    r1 += analogRead(0) + analogRead(1) + analogRead(2);
  }
  randomSeed(r1);
}

void setup() {
  setupRandomSeed();

  
  initAgentList();
  leds.begin();
  pinMode(sanityPin, OUTPUT);
  digitalWrite(sanityPin, sanityLED);
  sanityNextSwitch = millis() + sanityDelay;
}


void loop() {
  memset(&buffer[0], 0, sizeof(buffer));
  clear();
  //  newAgent();
  //  newAgent();
  //  newAgent();
  //  newAgent();



  int r = random(100);
  if (r < 90) {
    newAgent();
  } else if (r < 95) {
        foo();
        bar();
  } else if (r < 96) {
    baz();
  }

  //  if (frame % (frameRate * 1) == 0) {
  //    foo();
  //  }

  updateAgents();
  bufferToLEDs();
  displayLEDs();
  ++frame;
}


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
