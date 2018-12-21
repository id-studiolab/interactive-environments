#include <FastLED.h>
#include <SoftwareSerial.h>

FASTLED_USING_NAMESPACE

SoftwareSerial dSerial(10, 11); //RX TX

#define DATA_PIN    3
#define LED_TYPE    WS2813
#define COLOR_ORDER GRB
//#define COLOR_ORDER RGB

#define NUM_LEDS    70
CRGB leds[NUM_LEDS];

const int nrOfRows = 70;
byte lastColor[nrOfRows];
byte nextColor[nrOfRows];

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  //position on scale, r, g, b
  0,     0,  120,  160,   //petiBlue
  190,   0,  206,   10,    //petiGreen
  220, 0, 206, 10,
  240, 0, 103, 10,
  255,  0, 103,  10
};

CRGBPalette16 colorPalette = heatmap_gp;

int noPetF = 0;
int noPetA = 0;
String message = "";
char delimitKV = ':';
char delimitEnd = '!';
float lastRatio = 0.0;
float ratio = 0.0;

unsigned long lastLedUpdate = 0;

void setup() {
  delay(300); //for recovery
  Serial.begin(9600);
  dSerial.begin(9600);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  delay(500);

  ledStandard();
  delay(10);

  for (int i = 0; i < nrOfRows; i++) {
    lastColor[i] = 0;
    nextColor[i] = 110;
  }
  updateLed();
}

void ledStandard() {
  for (int i = 0; i < nrOfRows; i++) {
    int palletteValue = map(i, 0, nrOfRows, 0, 220);
    leds[i] = ColorFromPalette( colorPalette, palletteValue);

  }
  FastLED.show();
  delay(30);
}

void ledInactivePet(){
  for (int i = 0; i < nrOfRows; i++){
    lastColor[i]=nextColor[i];
    nextColor[i] = 240;
  }
}

void ledWithRatio(float ratio) { 
  byte focussed = ceil(ratio * nrOfRows);
  byte social = nrOfRows - focussed;
  for (int i = 0; i < nrOfRows; i++) {

    int paletteValue = 0;
    if (i < focussed) {
      paletteValue = map(i, 0, focussed, 0, 110);
    } else {
      paletteValue = map(i, focussed, nrOfRows, 110, 220);
    }
    lastColor[i] = nextColor[i];
    nextColor[i] = paletteValue;

  }
}

bool colorChange() {
  for (byte i = 0; i < nrOfRows; i++) {
    if (lastColor[i] != nextColor[i]) {
      return true;
    }
  }
  return false;
}

void updateLed () {
  while (colorChange()) {
    for (byte i = 0; i < nrOfRows; i++) {
      if (lastColor[i] < nextColor[i]) {
        lastColor[i]++;
      } else if (lastColor[i] > nextColor[i]) {
        lastColor[i]--;
      }
      leds[i] = ColorFromPalette( colorPalette, lastColor[i]);

    }

    FastLED.show();
    delay(30);
  }
}


void updateRatio() {
  lastRatio = ratio;
  if (noPetA != 0) {
    if (noPetF == 0){
      ratio = 0.1;
    } else {
    ratio = ((float)noPetF) / ((float) noPetA);
    }
  } else {
    ratio = 0;
  } if (ratio == 1.0) {
    ratio = 0.9;
  }
}
void takeInMessage() {
  int indexDelimitKV = message.indexOf(delimitKV);
  String key = message.substring(0, indexDelimitKV);
  int value = (message.substring(indexDelimitKV + 1)).toInt();
  Serial.println(message);

  if (key.startsWith("p")) {
    if (key.equals("pa")) {
      noPetA = value;
    } else if (key.equals("pf")) {
      noPetF = value;
    }
    updateRatio();
  }
}
void readFromdSerial() {
  if (dSerial.available()) {
    char r = dSerial.read();
    if (r == delimitEnd) {
      takeInMessage();
      message = "";
    } else {
      message += r;
    }
  }
}

void ledRed() {
  for (byte i = 0; i < nrOfRows; i++) {
    leds[i] = CRGB(155, 155, 0);
  }
  delay(30);
  FastLED.show();
}

void loop() {
  readFromdSerial();
  if (ratio != lastRatio){
        Serial.println(ratio);
    if (noPetA != 0){
       ledWithRatio(ratio);
    } else {
       ledInactivePet();
    }

    updateLed();
  }

}
