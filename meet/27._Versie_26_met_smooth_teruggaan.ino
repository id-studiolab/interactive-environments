// ---------------
// -- Libraries --
// ---------------

/*********************************************************
  MPR121 Library
**********************************************************/

#include <Wire.h>
#include "Adafruit_MPR121.h"
#ifndef _BV
#define _BV(bit) (1 << (bit))
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


#define INNER_PIN     2
#define MIDDLE_PIN    3
#define OUTER_PIN     4
#define TIME_PIN      13
#define INNER_PIXELS  33
#define OUTER_PIXELS  66
#define TIME_PIXELS   8
CRGB innerpixels[INNER_PIXELS];
CRGB outerpixels[OUTER_PIXELS];
CRGB timepixels[TIME_PIXELS];
#define BRIGHTNESS    255

// ---------------
// -- Variables --
// ---------------

#define NUM_PADS1     12
#define NUM_PADS2     12
#define NUM_PADS3     9
#define TOTAL_PADS    33
#define NUM_SUBJECTS  5
#define BUTTON_PIN1   6
#define BUTTON_PIN12  7
#define BUTTON_PIN2   8
#define BUTTON_PIN3   9
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
  {223, 255, 255}, //roze
  {136, 237, 255}, //blauw
  {37, 227, 255}, //geel
  {101, 221, 255}, //groen
  {193, 204, 255}, //paars
};

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
byte currentState = stateSetup;

#define delayVal  400

int nowLed;
byte dimValue;
unsigned long lastDim;
float timeSegment = 900000; // 900000 is 15 min
float timeSegmentsOn;                                                         // TIJD DOORKRIJGEN VAN TOM (when??)
float totalTime() {
  return timeSegment * timeSegmentsOn;
}
#define dimSegments  60
float timePerDim() {
  return totalTime() / (dimSegments * NUM_BOXES);
}
byte nextMeeting = 8;                                                         // TIJD DOORKRIJGEN VAN TOM: 1 is meeting over een kwartier, 7 is meeting over 7 kwartier, 8 is als er geen meeting in de weg van de 2 uur zit

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

  setStartValues();
  setStartSubjects();
  setSubjectsUsed();
  changeLedColors();
  turnLedsOn();
}

void loop() {
  updateStateMachine();
  // constant tijd met tom updaten??
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
    return true;
  }
  else {
    return false;
  }
}

boolean plusButtonPressed() {
  if (digitalRead(BUTTON_PIN3) == HIGH) {
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

// -------------
// -- Actions --
// -------------

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
// ^debug println?

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
    if ((currentTouch != previousTouch) && (previousTouch == subject[i].beginLed) && (abs(previousTouch - currentTouch) < 4)) {
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
    if ((currentTouch != previousTouch) && (previousTouch == subject[i].endLed) && (abs(previousTouch - currentTouch) < 4)) {
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
  if (millis() - lastDim > timePerDim()) {
    led[nowLed].brightness = 255 - (180 / dimSegments) * (dimValue + 1);
    dimValue ++;
    lastDim = millis();
  }

  if (dimValue == dimSegments) {
    led[nowLed].brightness = 0;
    dimValue = 0;
    nowLed ++;
  }
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

void changeTotalTime() {
  if (minusButtonPressed()) {
    if (timeSegmentsOn > 1) {
      timeSegmentsOn --;
    }
    delay(delayVal);
  }

  if (plusButtonPressed()) {
    if ((timeSegmentsOn < 8) && (timeSegmentsOn < nextMeeting)) {
      timeSegmentsOn ++;
    }
    delay(delayVal);
  }
}

void allLedsGreen() {
  for (int i = 0; i < NUM_BOXES; i++) {
    led[i].hue = endColorHue;
    led[i].saturation = endColorSaturation;
    led[i].brightness = endColorBrightness;
  }
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
      turnLedsOn();
      if (middleButtonPressed()) {
        lastDim = millis();
        nowLed = 0;
        delay(delayVal);
        currentState = stateCountDown;
      }
      break;

    case stateCountDown:
      // changeTotalTime(); en dan ook dimleds en rest heen/terug mee switchen!
      // naar tom info sturen als er meer tijd wordt bijgeklikt / het uit gaat lopen
      checkTouches();
      scaleLeds();
      changeLedColors();
      dimLeds();
      turnLedsOn();
      if (middleButtonPressed()) {
        if (currentSubject == subjectsUsed - 1) {
          delay(delayVal);
          lastClicked = millis();
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
      allLedsGreen();
      turnLedsOn();
      if ((lastClicked + 1500 < millis()) && (middleButtonPressed())) {
        setStartValues();
        setStartSubjects();
        setSubjectsUsed();
        changeLedColors();
        turnLedsOn();
        delay(delayVal);
        currentState = stateSetup;
      }
      // sort of reward when done on time?
      break;
  }
}
