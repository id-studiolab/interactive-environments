// ---------------
// -- Libraries --
// ---------------

/*********************************************************
  MPR121 Library
**********************************************************/

#include <Wire.h>
#include "Adafruit_MPR121.h"
#ifndef _BV
#endif

Adafruit_MPR121 cap1 = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();
Adafruit_MPR121 cap3 = Adafruit_MPR121();

#define touchThreshold    10
#define releaseThreshold  4

/*********************************************************
  FastLED Library
**********************************************************/
#include <FastLED.h>
#define _BV(bit) (1 << (bit))


#define INNER_PIN     2
#define MIDDLE_PIN    3
#define OUTER_PIN     5
#define TIME_PIN      13
#define INNER_PIXELS  33
#define OUTER_PIXELS  66
#define TIME_PIXELS   8
CRGB innerpixels[INNER_PIXELS];
CRGB outerpixels[OUTER_PIXELS];
CRGB timepixels[TIME_PIXELS];
#define BRIGHTNESS    100

/*********************************************************
  LEDBackpack Library
**********************************************************/

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

// ---------------
// -- Variables --
// ---------------

#define NUM_PADS1     12
#define NUM_PADS2     12
#define NUM_PADS3     9
#define TOTAL_PADS    33
#define NUM_SUBJECTS  6
#define BUTTON_PIN1   6
#define BUTTON_PIN12  7
#define BUTTON_PIN2   9
#define BUTTON_PIN3   8
#define NUM_BOXES     33

int lastTouched1 = 0;
int currTouched1 = 0;
int lastTouched2 = 0;
int currTouched2 = 0;
int lastTouched3 = 0;
int currTouched3 = 0;
byte currentTouch;
byte previousTouch;

int directionValue;

boolean executeAction = LOW;

unsigned long lastTimeTouched[TOTAL_PADS];
int debounceTime = 400;

byte colorIndex;

byte subjectsUsed;
byte subjectStartsize;

byte hsvColors [NUM_SUBJECTS][3] = {
  {120, 255, 255},
  {135, 255, 255},
  {185, 255, 255},
  {210, 255, 255},
  {240, 255, 255},
  {20, 235, 255},
};

//byte hsvColors [NUM_SUBJECTS][3] = {
//  {223, 255, 255}, //roze
//  {138, 255, 255}, //blauw
//  {37, 227, 255}, //geel
//  {106, 255, 255}, //groen
//  {193, 204, 255}, //paars
//  {0, 250, 255}, //rood
//};

#define usedHue 0
#define usedSaturation 0
#define usedBrightness 255
#define unusedHue 0
#define unusedSaturation 0
#define unusedBrightness 0
#define occupiedHue 0
#define occupiedSaturation 252
#define occupiedBrightness 226

#define endColorHue 101
#define endColorSaturation 221
#define endColorBrightness 255

struct led {
  byte hue = 0;
  byte saturation = 0;
  byte brightness = 0;
} led [NUM_BOXES];

struct subject {
  byte beginLed;
  byte endLed;
  byte hsvColor[3];
  boolean inUse = LOW;
} subject [NUM_SUBJECTS];

#define stateSetup      1
#define stateCountDown  2
#define stateDone       3
#define stateOverTime   4
byte currentState = stateSetup;

#define delayVal  400

int nowLed;
byte dimValue;
unsigned long lastDim;
#define timeSegmentMin 15
long timeSegmentMs = timeSegmentMin * 60000; //60000 moet eig
long timeSegmentsOn;                                                         // TIJD DOORKRIJGEN VAN TOM (when??)
long totalTime() {
  return timeSegmentMs * timeSegmentsOn;
}
#define dimSegments  60
unsigned long timePerDim;
byte nextMeeting = 8;                                                       // TIJD DOORKRIJGEN VAN TOM: 1 is meeting over een kwartier, 7 is meeting over 7 kwartier, 8 is als er geen meeting in de weg van de 2 uur zit

byte padTouched[TOTAL_PADS];
boolean padsOff;
unsigned long lastTime;

boolean changeSubjects;
byte currentSubject;

float calcSubjectSizes[NUM_SUBJECTS];
byte newSubjectSizes[NUM_SUBJECTS];
float remainingLeds;
float totalCalcSizes;

unsigned long lastClicked;
unsigned long lastLEDUpdate;

long hours;
long minutes;
unsigned long startCountdown;
unsigned long timeRemaining;

int plusminus = 0;
int aDelay = 1000;
boolean timeChanged = false;
boolean celebrations = false;

// --------------------
// -- Debug variables --
// --------------------

//#define DEBUG_TOUCH
//#define DEBUG_CHANGE
//#define DEBUG_BORDERS
//#define DEBUG_SCALING

#ifdef DEBUG_TOUCH
#define DEBUG_TOUCH_PRINTLN(x) Serial.println (x)
#define DEBUG_TOUCH_PRINT(x) Serial.print (x)
#else
#define DEBUG_TOUCH_PRINTLN(x)
#define DEBUG_TOUCH_PRINT(x)
#endif

#ifdef DEBUG_CHANGE
#define DEBUG_CHANGE_PRINTLN(x) Serial.println (x)
#define DEBUG_CHANGE_PRINT(x) Serial.print (x)
#else
#define DEBUG_CHANGE_PRINTLN(x)
#define DEBUG_CHANGE_PRINT(x)
#endif

#ifdef DEBUG_BORDERS
#define DEBUG_BORDERS_PRINTLN(x) Serial.println (x)
#define DEBUG_BORDERS_PRINT(x) Serial.print (x)
#else
#define DEBUG_BORDERS_PRINTLN(x)
#define DEBUG_BORDERS_PRINT(x)
#endif

#ifdef DEBUG_SCALING
#define DEBUG_SCALING_PRINTLN(x) Serial.println (x)
#define DEBUG_SCALING_PRINT(x) Serial.print (x)
#else
#define DEBUG_SCALING_PRINTLN(x)
#define DEBUG_SCALING_PRINT(x)
#endif


// ------------------
// -- Setup & Loop --
// ------------------

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }

  Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap1.begin(0x5A)) {
    Serial.println("MPR121  1  not found, check wiring?");
    while (1);
  }
  cap1.writeRegister(MPR121_ECR, 0x0);
  cap1.setThresholds(touchThreshold, releaseThreshold);
  cap1.writeRegister(MPR121_ECR, 0x8F);
  Serial.println("MPR121  1  found!");

  if (!cap2.begin(0x5B)) {
    Serial.println("MPR121  2  not found, check wiring?");
    while (1);
  }
  cap2.writeRegister(MPR121_ECR, 0x0);
  cap2.setThresholds(touchThreshold, releaseThreshold);
  cap2.writeRegister(MPR121_ECR, 0x8F);
  Serial.println("MPR121  2  found!");

  if (!cap3.begin(0x5C)) {
    Serial.println("MPR121  3  not found, check wiring?");
    while (1);
  }
  cap3.writeRegister(MPR121_ECR, 0x0);
  cap3.setThresholds(touchThreshold, releaseThreshold);
  cap3.writeRegister(MPR121_ECR, 0x8F);
  Serial.println("MPR121  3  found!");

  FastLED.addLeds<NEOPIXEL, INNER_PIN>(innerpixels, INNER_PIXELS);
  FastLED.addLeds<NEOPIXEL, MIDDLE_PIN>(outerpixels, OUTER_PIXELS);
  FastLED.addLeds<NEOPIXEL, OUTER_PIN>(outerpixels, OUTER_PIXELS);
  FastLED.addLeds<NEOPIXEL, TIME_PIN>(timepixels, TIME_PIXELS);

  FastLED.setBrightness(BRIGHTNESS);

#ifndef __AVR_ATtiny85__
  Serial.println("7 Segment Backpack Test");
#endif
  matrix.begin(0x70);

  wipeLeds();

  setStartValues();
  setStartSubjects();
  setSubjectsUsed();
  changeLedColors();
  turnLedsOn();
  turnScreenOn();
}

void loop() {
  updateStateMachine();
/*
  if (Serial1.available() > 0) {
    String timeslot = Serial1.readStringUntil('\n');
    String meetduration = getValue(timeslot, '.', 0);
    String timetillnext = getValue(timeslot, '.', 1);
    int duration = meetduration.toInt();
    int tillnext = timetillnext.toInt();
//    Serial.println(duration);
//    Serial.println(tillnext);
  }*/
}

// ----------------
// -- Conditions --
// ----------------

boolean middleButtonPressed() {
  if (digitalRead(BUTTON_PIN1) == HIGH || digitalRead(BUTTON_PIN12) == HIGH) {
    return true;
  }
  else {
    return false;
  }
}

boolean minusButtonPressed() {
  if (digitalRead(BUTTON_PIN2) == HIGH) {
    timeChanged = true;
    return true;
  }
  else {
    return false;
  }
}

boolean plusButtonPressed() {
  if (digitalRead(BUTTON_PIN3) == HIGH) {
    timeChanged = true;
    return true;
  }
  else {
    return false;
  }
}

boolean checkPads() {
  int total = 0;
  for (int i = 0; i < TOTAL_PADS; i++) {
    total += padTouched[i];
  }
  return total;
}

boolean shouldUpdateLEDs(int updateDelay) {
  if (millis() > lastLEDUpdate + updateDelay) {
    lastLEDUpdate = millis();
    return true;
  }
  return false;
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// -------------
// -- Actions --
// -------------


void wipeLeds() {
    for(int i = 0; i < TIME_PIXELS; i++) {
    timepixels[i] = CHSV(130, 0, 255);
    delay(5);
    FastLED.show();
  }

  for(int i = 0; i < NUM_BOXES; i++) {
    outerpixels[2*i] = CHSV(130, 204, 255);
    outerpixels[2*i + 1] = CHSV(130, 204, 255);
    innerpixels[i] = CHSV(130, 204, 255);
    delay(5);
    FastLED.show();
  }
  delay(1000);
}

void debug() {
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap1.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i = 0; i < NUM_PADS1; i++) {
    Serial.print(cap1.filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i = 0; i < NUM_PADS1; i++) {
    Serial.print(cap1.baselineData(i)); Serial.print("\t");
  }
  Serial.println();

  // put a delay so it isn't overwhelming
  delay(100);

  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap2.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i = 0; i < NUM_PADS2; i++) {
    Serial.print(cap2.filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i = 0; i < NUM_PADS2; i++) {
    Serial.print(cap2.baselineData(i)); Serial.print("\t");
  }
  Serial.println();

  // put a delay so it isn't overwhelming
  delay(100);

  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap3.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i = 0; i < NUM_PADS3; i++) {
    Serial.print(cap3.filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i = 0; i < NUM_PADS3; i++) {
    Serial.print(cap3.baselineData(i)); Serial.print("\t");
  }
  Serial.println();

  // put a delay so it isn't overwhelming
  delay(100);
}

void setStartValues() {
  nowLed = -1;
  dimValue = 0;
  timeSegmentsOn = 4;
  subjectsUsed = 3;
  currentSubject = 0;
  timeRemaining = 3600000;
  celebrations = false;
  timeChanged = false;
}

void setStartSubjects() {
  subjectStartsize = NUM_BOXES / subjectsUsed;

  for (int i = 0; i < NUM_SUBJECTS; i++) {
    subject[i].beginLed = 0 + i * subjectStartsize;
    subject[i].endLed = subject[i].beginLed + subjectStartsize -  1;
    Serial.print(subject[i].beginLed); Serial.print("   "); Serial.println(subject[i].endLed);

    subject[i].hsvColor[0] = hsvColors[i][0];
    subject[i].hsvColor[1] = hsvColors[i][1];
    subject[i].hsvColor[2] = hsvColors[i][2];
  }
}

void beginLedPosChange(int index) {
  DEBUG_CHANGE_PRINTLN("Begin Led Positive Change");

  subject[index].beginLed = currentTouch;
  subject[index - 1].endLed = currentTouch - 1;

  for (int i = index; i < NUM_SUBJECTS; i++) {
    if (subject[i].beginLed == subject[i].endLed) {
      subject[i].endLed ++;
      subject[i + 1].beginLed ++;
    }
  }
}

void beginLedNegChange(int index) {
  DEBUG_CHANGE_PRINTLN("Begin Led Negative Change");

  for (int i = index - 1; i >= 0; i--) {
    if (subject[i].endLed != subject[i].beginLed + 1) {
      executeAction = true;
      break;
    }
  }

  if (executeAction) {
    subject[index].beginLed = currentTouch;
    subject[index - 1].endLed = currentTouch - 1;

    for (int i = index - 1; i >= 0; i--) {
      if (subject[i].beginLed == subject[i].endLed) {
        subject[i].beginLed --;
        subject[i - 1].endLed --;
      }
    }
    executeAction = false;
  }
}

void endLedPosChange(int index) {
  DEBUG_CHANGE_PRINTLN("End Led Positive Change");

  subject[index].endLed = currentTouch;
  subject[index + 1].beginLed = currentTouch + 1;

  for (int i = index + 1; i < NUM_SUBJECTS; i++) {
    if (subject[i].beginLed == subject[i].endLed) {
      subject[i].endLed ++;
      subject[i + 1].beginLed ++;
    }
  }
}

void endLedNegChange(int index) {
  DEBUG_CHANGE_PRINTLN("End Led Negative Change");

  for (int i = index; i >= 0; i--) {
    if (subject[i].endLed != subject[i].beginLed + 1) {
      executeAction = true;
      break;
    }
  }

  if (executeAction) {
    subject[index].endLed = currentTouch;
    subject[index + 1].beginLed = currentTouch + 1;

    for (int i = index; i >= 0; i--) {
      if (subject[i].beginLed == subject[i].endLed) {
        subject[i].beginLed --;
        subject[i - 1].endLed --;
      }
    }
    executeAction = false;
  }
}

void lastBeginLedNegChange(int index) {
  DEBUG_CHANGE_PRINTLN("Last Begin Led Negative Change");

  for (int i = index - 1; i >= 0; i--) {
    if (subject[i].endLed != subject[i].beginLed + 1) {
      executeAction = true;
      break;
    }
  }

  if (executeAction) {
    subject[index].beginLed = currentTouch;
    subject[index - 1].endLed = currentTouch - 1;
    subject[index].endLed = NUM_BOXES - 1;

    for (int i = index - 1; i >= 0; i--) {
      if (subject[i].beginLed == subject[i].endLed) {
        subject[i].beginLed --;
        subject[i - 1].endLed --;
      }
    }
    executeAction = false;
  }
}

void addSubject() {
  DEBUG_CHANGE_PRINTLN("Add Subject");

  for (int i = subjectsUsed; i >= 0; i--) {
    if (subject[i].endLed != subject[i].beginLed + 1) {
      executeAction = true;
      break;
    }
  }

  if (executeAction && (subjectsUsed < NUM_SUBJECTS)) {
    if (subject[subjectsUsed - 1].beginLed == NUM_BOXES - 1) {
      subject[subjectsUsed - 1].endLed = currentTouch - 1;
      subject[subjectsUsed - 1].beginLed = currentTouch - 2;
      subject[subjectsUsed].beginLed = currentTouch;
      subject[subjectsUsed].endLed = currentTouch + 1;
    } else {
      subject[subjectsUsed - 1].endLed = currentTouch - 1;
      subject[subjectsUsed].beginLed = currentTouch;
      subject[subjectsUsed].endLed = currentTouch + 1;
    }

    for (int i = subjectsUsed - 1; i >= 0; i--) {
      if (subject[i].beginLed == subject[i].endLed) {
        subject[i].beginLed --;
        subject[i - 1].endLed --;
      }
    }
  }
  executeAction = false;
}

void removeSubject() {
  DEBUG_CHANGE_PRINTLN("Remove Subject");

  subject[subjectsUsed - 1].beginLed = NUM_BOXES;
  subject[subjectsUsed - 1].endLed = NUM_BOXES + 1;
  subject[subjectsUsed - 2].endLed = NUM_BOXES - 1;
}

void checkTouches() {
  currTouched1 = cap1.touched();                                      // FIRST: read out current touches from sensors
  currTouched2 = cap2.touched();
  currTouched3 = cap3.touched();

  for (int i = 0; i < NUM_PADS1; i++) {                                 // SECOND: say what is touched and what is released
    if (millis() - lastTimeTouched[i] > debounceTime) {
      if ((currTouched1 & _BV(i)) && !(lastTouched1 & _BV(i)) ) {
        DEBUG_TOUCH_PRINT(i); DEBUG_TOUCH_PRINT(" touched,   "); DEBUG_TOUCH_PRINT(previousTouch); DEBUG_TOUCH_PRINTLN(" previousTouch");
        currentTouch = i;
        lastTimeTouched[i] = millis();
        padTouched[i] = HIGH;
      }
    }
    if (!(currTouched1 & _BV(i)) && (lastTouched1 & _BV(i)) ) {
      DEBUG_TOUCH_PRINT(i); DEBUG_TOUCH_PRINT(" released,   "); DEBUG_TOUCH_PRINT(previousTouch); DEBUG_TOUCH_PRINTLN(" previousTouch");
      padTouched[i] = LOW;
    }
  }

  for (int i = 0; i < NUM_PADS2; i++) {
    if (millis() - lastTimeTouched[i + NUM_PADS1] > debounceTime) {
      if ((currTouched2 & _BV(i)) && !(lastTouched2 & _BV(i)) ) {
        DEBUG_TOUCH_PRINT(i + NUM_PADS1); DEBUG_TOUCH_PRINT(" touched,   "); DEBUG_TOUCH_PRINT(previousTouch); DEBUG_TOUCH_PRINTLN(" previousTouch");
        currentTouch = i + NUM_PADS1;
        lastTimeTouched[i + NUM_PADS1] = millis();
        padTouched[i + NUM_PADS1] = HIGH;
      }
    }
    if (!(currTouched2 & _BV(i)) && (lastTouched2 & _BV(i)) ) {
      DEBUG_TOUCH_PRINT(i + NUM_PADS1); DEBUG_TOUCH_PRINT(" released,   "); DEBUG_TOUCH_PRINT(previousTouch); DEBUG_TOUCH_PRINTLN(" previousTouch");
      padTouched[i + NUM_PADS1] = LOW;
    }
  }

  for (int i = 0; i < NUM_PADS3; i++) {
    if (millis() - lastTimeTouched[i + NUM_PADS1 + NUM_PADS2] > debounceTime) {
      if ((currTouched3 & _BV(i)) && !(lastTouched3 & _BV(i)) ) {
        DEBUG_TOUCH_PRINT(i + NUM_PADS1 + NUM_PADS2); DEBUG_TOUCH_PRINT(" touched,   "); DEBUG_TOUCH_PRINT(previousTouch); DEBUG_TOUCH_PRINTLN(" previousTouch");
        currentTouch = i + NUM_PADS1 + NUM_PADS2;
        lastTimeTouched[i + NUM_PADS1 + NUM_PADS2] = millis();
        padTouched[i + NUM_PADS1 + NUM_PADS2] = HIGH;
      }
    }
    if (!(currTouched3 & _BV(i)) && (lastTouched3 & _BV(i)) ) {
      DEBUG_TOUCH_PRINT(i + NUM_PADS1 + NUM_PADS2); DEBUG_TOUCH_PRINT(" released,   "); DEBUG_TOUCH_PRINT(previousTouch); DEBUG_TOUCH_PRINTLN(" previousTouch");
      padTouched[i + NUM_PADS1 + NUM_PADS2] = LOW;
    }
  }

  if (checkPads() == 0) {                                        // THIRD: if nothing is touched for a certain time, reset the current touch to a nonexistent value
    if (padsOff) {
      if (millis() - lastTime > 200) {
        currentTouch = 70;
      }
    } else {
      padsOff = true;
      lastTime = millis();
    }
  } else {
    padsOff = false;
  }                                                               // FOURTH: check if there is movement from a beginLed to somewhere

  for (int i = 1; i < subjectsUsed; i++) {
    if ((currentTouch != previousTouch) && (previousTouch == subject[i].beginLed) && (abs(previousTouch - currentTouch) < 3)) {
      directionValue = currentTouch - previousTouch;
      DEBUG_CHANGE_PRINT("BeginLED     "); DEBUG_CHANGE_PRINTLN(directionValue);
      if (directionValue > 0) {
        beginLedPosChange(i);
      }
      if (directionValue < 0) {
        if (i == subjectsUsed - 1) {
          lastBeginLedNegChange(i);
        }
        else {
          beginLedNegChange(i);
        }
      }
      lastTouched1 = currTouched1;
      lastTouched2 = currTouched2;
      lastTouched3 = currTouched3;
      previousTouch = currentTouch;
      setSubjectsUsed();
    }
  }                                                           // FIFTH: check if there is movement from an endLed to somewhere
  for (int i = 0; i < subjectsUsed - 1; i++) {
    if ((currentTouch != previousTouch) && (previousTouch == subject[i].endLed) && (abs(previousTouch - currentTouch) < 3)) {
      directionValue = currentTouch - previousTouch;
      DEBUG_CHANGE_PRINT("EndLED     "); DEBUG_CHANGE_PRINTLN(directionValue);
      if (directionValue > 0) {
        endLedPosChange(i);
      }
      if (directionValue < 0) {
        endLedNegChange(i);
      }
      lastTouched1 = currTouched1;
      lastTouched2 = currTouched2;
      lastTouched3 = currTouched3;
      previousTouch = currentTouch;
      setSubjectsUsed();
    }
  }                                                            // SIXTH: remove and add subject --> movement with the last pad

  if ((currentTouch != previousTouch) && (previousTouch == 0) && (currentTouch == TOTAL_PADS - 1)) {
    addSubject();
    setSubjectsUsed();
  }

  if ((currentTouch != previousTouch) && (previousTouch == TOTAL_PADS - 1) && (currentTouch == 0) && (previousTouch == subject[subjectsUsed - 1].beginLed)) {
    removeSubject();
    setSubjectsUsed();
  }                                                           // SEVENTH: set the current touch in the previous one
  lastTouched1 = currTouched1;
  lastTouched2 = currTouched2;
  lastTouched3 = currTouched3;
  previousTouch = currentTouch;
}

void setSubjectsUsed() {
  subjectsUsed = 0;
  for (int i = 0; i < NUM_SUBJECTS; i++) {
    if (subject[i].beginLed < NUM_BOXES && subject[i].beginLed >= 0) {
      subject[i].inUse = HIGH;
      subjectsUsed ++;
    }
    else {
      subject[i].inUse = LOW;
    }
  }

  if (subject[subjectsUsed - 1].beginLed < NUM_BOXES - 1) {
    subject[subjectsUsed - 1].endLed = NUM_BOXES - 1;
  }

  for (int i = 0; i < NUM_SUBJECTS; i++) {
    calcSubjectSizes[i] = subject[i].endLed - subject[i].beginLed;

    if (subject[i].beginLed != NUM_BOXES - 1) {
      calcSubjectSizes[i] ++;
    }
  }

  DEBUG_CHANGE_PRINT("subjectsUsed:     "); DEBUG_CHANGE_PRINTLN(subjectsUsed);
  printSubjectBorders();
}

void printSubjectBorders() {
  for (int i = 0; i < NUM_SUBJECTS; i++) {
    String debug1 = "";
    debug1 = debug1 + i + ": begin: " + subject[i].beginLed + " end: " + subject[i].endLed + " Calcsize: " + calcSubjectSizes[i] + " Newsize: " + newSubjectSizes[i];
    DEBUG_BORDERS_PRINTLN(debug1);
  }
}

void changeLedColors() {
  for (int i = 0; i < subjectsUsed; i++) {
    for (int j = subject[i].beginLed; j <= subject[i].endLed; j++) {
      if  (j < NUM_BOXES) {
        led[j].hue = subject[i].hsvColor[0];
        led[j].saturation = subject[i].hsvColor[1];
        if (j > nowLed) {
          led[j].brightness = subject[i].hsvColor[2];
        }
      }
    }
  }
}

void turnLedsOn() {
  for (int i = 0; i < NUM_BOXES; i++) {
    outerpixels[2 * i] = CHSV(led[i].hue, led[i].saturation, led[i].brightness);
    outerpixels[2 * i + 1] = CHSV(led[i].hue, led[i].saturation, led[i].brightness);
    innerpixels[i] = CHSV(led[i].hue, led[i].saturation, led[i].brightness);
  }

  for (int i = 0; i < timeSegmentsOn; i++) {
    timepixels[i] = CHSV(usedHue, usedSaturation, usedBrightness);
  }

  for (int i = timeSegmentsOn; i < nextMeeting; i++) {
    timepixels[i] = CHSV(unusedHue, unusedSaturation, unusedBrightness);
  }

  if (nextMeeting < 8) {
    for (int i = nextMeeting; i < TIME_PIXELS; i++) {
      timepixels[i] = CHSV(occupiedHue, occupiedSaturation, occupiedBrightness);
    }
  }
  if (shouldUpdateLEDs(20)) {
    FastLED.show();
  }
}

void dimLeds() {
  if (millis() - lastDim > timePerDim) {
    led[nowLed].brightness = 255 - (180 / dimSegments) * (dimValue + 1);
    dimValue ++;
    lastDim = millis();
  }

  if (dimValue == dimSegments) {
    led[nowLed].brightness = 0;
    dimValue = 0;
    nowLed ++;
  }

//  Serial.print(nowLed); Serial.print(" , "); Serial.print(dimValue); Serial.print(" , "); Serial.print(millis()-lastDim); Serial.print(" , "); Serial.println(timePerDim);
}

void scaleLeds() {
  if (changeSubjects == HIGH) {
    DEBUG_SCALING_PRINTLN(currentSubject);
    if (nowLed < subject[currentSubject].endLed) {
      currentSubject ++;
      remainingLeds = NUM_BOXES - nowLed;
      subject[currentSubject - 1].endLed = nowLed - 1;

      if (nowLed == 0) {
        subject[currentSubject - 1].beginLed = - 2;
      }

      for (int i = currentSubject; i < subjectsUsed; i++) {
        totalCalcSizes += calcSubjectSizes[i];
      }

      for (int i = currentSubject; i < subjectsUsed; i++) {
        newSubjectSizes[i] = calcSubjectSizes[i] / totalCalcSizes * remainingLeds;
      }
      setNewSubjectSizes1();
      DEBUG_SCALING_PRINTLN("SCENARIO 1!!!");
    }
    else if (nowLed == subject[currentSubject].endLed) {
      currentSubject ++;
      subject[currentSubject - 1].endLed = nowLed - 1;
      subject[currentSubject].beginLed = nowLed;
      DEBUG_SCALING_PRINTLN("SCENARIO 3!!!");
    }
    changeSubjects = LOW;
    totalCalcSizes = 0;
    printSubjectBorders();
  }

  if (nowLed > subject[currentSubject].endLed) {
    subject[currentSubject].endLed = nowLed;

    remainingLeds = NUM_BOXES - nowLed - 1;

    for (int i = currentSubject + 1; i < subjectsUsed; i++) {
      totalCalcSizes += calcSubjectSizes[i];
    }

    for (int i = currentSubject + 1; i < subjectsUsed; i++) {
      newSubjectSizes[i] = calcSubjectSizes[i] / totalCalcSizes * remainingLeds;
      if (newSubjectSizes[i] < 2) {
        newSubjectSizes[i] = 2;
      }
    }
    setNewSubjectSizes2();

    totalCalcSizes = 0;
    printSubjectBorders();
    DEBUG_SCALING_PRINTLN("SCENARIO 2!!!");
  }
  // zorgen dat een beginled en endled niet terug mag tijdens uitlopen, alleen maar plus
  // alles wat over the edge heen wordt gepusht knipperend / donkerder door laten lopen naar het begin???
}

void setNewSubjectSizes1() {
  for (int i = currentSubject; i < subjectsUsed; i++) {
    subject[i].beginLed = subject[i - 1].endLed + 1;
    subject[i].endLed = subject[i].beginLed + newSubjectSizes[i] - 1;
  }
  subject[subjectsUsed - 1].endLed = NUM_BOXES - 1;
}

void setNewSubjectSizes2() {
  for (int i = currentSubject + 1; i < subjectsUsed; i++) {
    subject[i].beginLed = subject[i - 1].endLed + 1;
    subject[i].endLed = subject[i].beginLed + newSubjectSizes[i] - 1;
  }
  subject[subjectsUsed - 1].endLed = NUM_BOXES - 1;
}

void countdownTotalTime() {
  timeRemaining = totalTime() - (millis() - startCountdown);
}

void changeTotalTime() {
    if (minusButtonPressed()) {
      if ((timeSegmentsOn > 1) && (timeRemaining > timeSegmentMs)) {
        timeSegmentsOn --;
        Serial.println("minus");
        plusminus = 1;
      }
    }

    if (plusButtonPressed()) {
      if ((timeSegmentsOn < 8) && (timeSegmentsOn < nextMeeting)) {
        timeSegmentsOn ++;
        Serial.println("plus");
        plusminus = 2;
      }
    }

    if (timeChanged) {
      if (currentState == stateCountDown) {
        timePerDim = timeRemaining / ((NUM_BOXES - nowLed - 1) * dimSegments + (dimSegments - dimValue));
//        Serial.println(timePerDim);
        sendToDailyMeet();
      }
      plusminus = 0;
      timeChanged = false;
      delay(delayVal);
  }
}

void sendToDailyMeet() {
  if ((timeChanged == true) && (currentState == stateCountDown)) {
    Serial.println("STUUR P/M");
    String plusminusSend = "";
    plusminusSend += plusminus;
    Serial1.write(plusminusSend.c_str());
    Serial1.write('\n');
  }
  else if ((timeChanged == false) && (currentState == stateSetup)) {
    Serial.println("STUUR TOTAAL BEGIN");
    String timeonSend = "";
    timeonSend += timeSegmentsOn;
    Serial1.write(timeonSend.c_str());
    Serial1.write('\n');
  }
}

void turnScreenOn() {
  if (currentState == stateSetup) {
    hours = totalTime() / 3600000;
    minutes = (totalTime() % 3600000) / 60000;
  }
  else if (currentState == stateCountDown) {
    hours = timeRemaining / 3600000;
    minutes = (timeRemaining % 3600000) / 60000;
  }

    matrix.writeDigitNum(1, hours);
    matrix.writeDigitNum(3, minutes / 10);
    matrix.writeDigitNum(4, minutes % 10);
    matrix.drawColon(true);

    matrix.writeDisplay();
}

void allLedsGreen() {
  for (int i = 0; i < NUM_BOXES; i++) {
    led[i].hue = endColorHue;
    led[i].saturation = endColorSaturation;
    led[i].brightness = endColorBrightness;
  }
}

void celebrationYay() {
  if (celebrations) {
  for(int j = 0; j < NUM_SUBJECTS; j++) {
    for(int i = 0; i < TIME_PIXELS; i++) {
      timepixels[i] = CHSV(hsvColors[j][0], hsvColors[j][1], hsvColors[j][2]);
      delay(5);
      FastLED.show();
    }

    for(int i = 0; i < NUM_BOXES; i++) {
      outerpixels[2*i] = CHSV(hsvColors[j][0], hsvColors[j][1], hsvColors[j][2]);
      outerpixels[2*i + 1] = CHSV(hsvColors[j][0], hsvColors[j][1], hsvColors[j][2]);
      innerpixels[i] = CHSV(hsvColors[j][0], hsvColors[j][1], hsvColors[j][2]);
      delay(5);
      FastLED.show();
    }
  }
  celebrations = false;
  }
}

void goingOver() {
  Serial.println("GOING OVER");
  if (millis() - lastDim > (timePerDim/2)) {
    led[nowLed].hue = occupiedHue;
    led[nowLed].saturation = occupiedSaturation;
    led[nowLed].brightness = 75 + (180 / dimSegments) * (dimValue + 1);
    dimValue ++;
    lastDim = millis();
  }

  if (dimValue == dimSegments) {
    led[nowLed].brightness = 255;
    dimValue = 0;
    nowLed ++;
  }
}

void screenOnZero() {
    matrix.writeDigitNum(1, 0);
    matrix.writeDigitNum(3, 0);
    matrix.writeDigitNum(4, 0);
    matrix.drawColon(true);
    matrix.writeDisplay();
}

// --------------------
// -- Your functions --
// --------------------

void updateStateMachine() {

  switch (currentState) {

    case stateSetup:
      changeTotalTime();
      checkTouches();
      changeLedColors();
      turnScreenOn();
      turnLedsOn();
      if (middleButtonPressed()) {
        lastDim = millis();
        startCountdown = millis();
        nowLed = 0;
        timePerDim = totalTime() / (dimSegments * NUM_BOXES);
        Serial.println(timePerDim);
        sendToDailyMeet();
        delay(delayVal);
        currentState = stateCountDown;
      }
      break;

    case stateCountDown:
      changeTotalTime();
      countdownTotalTime();
      checkTouches();
      scaleLeds();
      changeLedColors();
      dimLeds();
      turnScreenOn();
      turnLedsOn();
      if (nowLed > NUM_BOXES) {
        currentState = stateOverTime;
        dimValue = 0;
        nowLed = 0;
      }
      if (middleButtonPressed()) {
        if (currentSubject == subjectsUsed - 1) {
          lastClicked = millis();
          delay(delayVal);
          celebrations = true;
          screenOnZero;
          currentState = stateDone;          
        }
        else {
          changeSubjects = HIGH;
          delay(delayVal);
        }
      }
      // what if time is out?? --> doorgaan in rood? of alles langzaam op en neer rood pulseren
      break;

    case stateDone:
      celebrationYay();
      if (middleButtonPressed()) { //(lastClicked + 1500 < millis()) &&
        setStartValues();
        setStartSubjects();
        setSubjectsUsed();
        changeLedColors();
        turnLedsOn();
        turnScreenOn();
        delay(delayVal);
        currentState = stateSetup;
      }
      // sort of reward when done on time?
      break;

    case stateOverTime:
      goingOver();
      screenOnZero();
      turnLedsOn();
      if (middleButtonPressed()) {
        celebrations = true;
        delay(delayVal);
        currentState = stateDone;
      }
  }
}
