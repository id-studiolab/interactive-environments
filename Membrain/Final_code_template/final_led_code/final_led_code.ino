#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            5
#define NUMPIXELS      70

#include <SoftwareSerial.h>

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int nLedColor[3];


//this is to keep track of the ledswap
long lastLedChangedTime;
int updateTime = 50;

//this is to keep track of the changing mode
long lastTimeModeChanged;
long changeModeInterval = 6000;
int activeMode = 0;
int nModes = 3;

//this is to randomly create a flas of light to simulate the water being watered
long lastWaterTime = 0;
int waterInterval = 1000;


int ledColors[70];

uint32_t black = pixels.Color(0, 0, 0);
uint32_t red = pixels.Color(255, 0, 0);
uint32_t green = pixels.Color(0, 255, 0);
uint32_t blue = pixels.Color(0, 0, 255);
uint32_t yellow = pixels.Color(255, 255, 0);
uint32_t aqua = pixels.Color(0, 255, 255);
uint32_t magenta = pixels.Color(255, 0, 255);
uint32_t white = pixels.Color(255, 255, 255);

uint32_t ColorArray[] = {black, red, green, blue, yellow, aqua, magenta, white};

int color1;
int color2;
int color3;


SoftwareSerial mySerial (9, 10);

void setup() {
  pixels.begin();

  Serial.begin (9600);
  initLedData(70, 30, 0, 2, 3, 0);

}

void loop() {

  if (millis() - lastTimeModeChanged > changeModeInterval) {
    activeMode++;
    turnOff();
    if (activeMode == nModes) {
      activeMode = 0;
    }

    switch (activeMode) {
      case 0:
        initLedData(70, 30, 0, 2, 3, 0);
        Serial.println("starting focus /relaxed mode");
        break;

      case 1:
        initLedData(30, 30, 30, 4, 5, 6);
        Serial.println("starting forest cycle");
        break;

      case 2:
        initLedData(25, 75, 0, 1, 7, 0);
        Serial.println("starting climate cycle");

        break;

      case 3:
        initLedData(75, 25, 0, 0, 0, 0);
        Serial.println("starting all climate");

        break;
    }
    lastTimeModeChanged = millis();
  }


  if (millis() - lastLedChangedTime > updateTime) {
    swapTwoColors();
    lastLedChangedTime = millis();
  }

  if (millis() - lastWaterTime > waterInterval) {
    flashFlash();
    lastWaterTime = millis();
    waterInterval = random (1000, 2000);
  }

  //printLedData();
  visualizeLed();
}

void initLedData(int data1, int data2, int data3, int color1, int color2, int color3) {

  color1 = color1;
  color2 = color2;
  color3 = color3;

  nLedColor[0] = map(data1, 0, 100, 0, NUMPIXELS);
  nLedColor[1] = map(data2, 0, 100, 0, NUMPIXELS);
  nLedColor[2] = NUMPIXELS - nLedColor[0] - nLedColor[1];

  int colorAssigned[3] = {0, 0, 0};

  for (int i = 0; i < NUMPIXELS; i++) {
    int randomColor = int(random(3));
    int sum = colorAssigned[0] + colorAssigned[1] + colorAssigned[2];

    if (colorAssigned[randomColor] < nLedColor[randomColor] ) {
      switch (randomColor) {
        case (0):
          ledColors[i] = color1;
          colorAssigned[0]++;
          break;
        case (1):
          ledColors[i] = color2;
          colorAssigned[1]++;
          break;
        case (2):
          ledColors[i] = color3;
          colorAssigned[2]++;
          break;
      }
    } else {
      i--;
    }
  }
}



void printLedData() {
  for (int i = 0; i < NUMPIXELS; i++) {
    Serial.print(ledColors[i]);
    Serial.print("-");
  }
  Serial.println();
}

void visualizeLed() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, ColorArray[ledColors[i]]);
  }
  pixels.show();
}

void swapTwoColors() {
  int random1Led = random (NUMPIXELS);
  int random2Led = random (NUMPIXELS);

  int color1 = ledColors[random1Led];
  int color2 = ledColors[random2Led];

  ledColors[random1Led] = color2;
  ledColors[random2Led] = color1;
}


void flashFlash() {
  for (int i = 0; i < 3; i++) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 255, 255));
      pixels.show();

    }
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
    }
  }
}

void turnOff() {
  for (int i = 0; i < 3; i++) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(20);
    }
  }
}

void parseCommandFromSerial() {
  if (mySerial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    activeMode = command.toInt();
  }
}



