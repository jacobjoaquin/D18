/*
  dataline.ino
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

// Agent
struct Agent {
  float position;
  uint8_t length;
  float direction;
  uint8_t color;
  uint8_t framesLeft;
  uint8_t nFrames;
};

uint8_t stripOrder[] = {0, 1, 2, 3, 7, 6, 5, 4};
// Constansts
const int ledsPerStrip = 150;
const int nStrips = 8;
const int nLeds = ledsPerStrip * nStrips;
const int frameRate = 60;
const int frameDelay = 1000 / frameRate;

// Octows2811 Setup
DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// User defined variables
uint32_t colorOrange = rgb(255, 64, 0);
uint32_t colorMagenta = rgb(255, 0, 92);
uint32_t colorBlack = rgb(0, 0, 0);
uint32_t colorWhite = rgb(255, 255, 255);
uint32_t colorPink = rgb(128, 0, 64);

const uint8_t black = 0;
const uint8_t orange = 1;
const uint8_t pink = 2;
const uint8_t white = 3;
uint32_t palette[4] = {colorBlack, colorOrange, colorPink, colorWhite};


// LED Buffer
uint32_t buffer[nLeds] = {0};

// Random buffer
uint8_t randomBuffer[nLeds] = {0};


// Sanity led
boolean sanityLED = true;
uint32_t sanityDelay = 48;
uint32_t sanityNextSwitch;
int sanityPin = 13;

const int nAgents = 1600;
Agent agentList[nAgents];
int agentLength = 5;
int agentFrames = 40;
int agentIndex = 0;

// Timing
ulong showTime = millis() + frameDelay;
uint32_t frame = 0;

void setup() {
  setupRandomSeed();
  pinMode(sanityPin, OUTPUT);
  digitalWrite(sanityPin, sanityLED);
  sanityNextSwitch = millis() + sanityDelay;
  initAgentList();
  leds.begin();
}


void loop() {
  // Reset temporary buffer
  memset(&buffer[0], 0, sizeof(buffer));

  // Fill randomBuffer with noise
  if (!(frame % 4)) {
    for (int i = 0; i < nLeds; ++i) {
      randomBuffer[i] = random(256);
    }
  }

  if (!(frame % 4)) {
    newAgent();
  }

  if (random(100) < 2) {
    //    foo();
  }

  if (!(frame % 120)) {
    cycleDisorient();
  }
  //  int r = random(100);
  //  if (r < 90) {
  //    newAgent();
  //  } else if (r < 95) {
  //    foo();
  //    bar();
  //  } else if (r < 96) {
  //    baz();
  //  }


  updateAgents();


    // Random strip flick
  if (!(frame % 12)) {
    int start = ledsPerStrip * random(nStrips);
    int nFlicker = ledsPerStrip;
    for (int i = start; i < start + nFlicker; ++i) {
      int index = i % nLeds;
      if (buffer[index] >= 1) {
        buffer[index] = palette[white];
      }
    }
  }

  bufferToLEDs();



  
  displayLEDs();
  ++frame;
}

