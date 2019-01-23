#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define rxPin 10
#define txPin 11

int buttonPins [4] = { 3, 4, 2, 19 };
int ledNumbers [4] = { 6, 6, 10, 12 };
int ledPins [4] = { 8, 9, 7, 6 };

int lastButtonStates[4];

Adafruit_NeoPixel pixels_FS = Adafruit_NeoPixel(ledNumbers[0], ledPins [0], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_forest = Adafruit_NeoPixel(ledNumbers[1], ledPins [1], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_climate = Adafruit_NeoPixel(ledNumbers[2], ledPins [2], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_playback = Adafruit_NeoPixel(ledNumbers[3], ledPins [3], NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel pixels_whiteStrip = Adafruit_NeoPixel(70, A4, NEO_GRB + NEO_KHZ800);


unsigned long lastActivityCheck = 0;
unsigned long lastSent = 0;

int activeMode = 0;

long intervalBetweenEachMode = 1000 * 60 * 2; //2 minutes
long lastTimeUpdated = 0;

void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(9600);
  //lolin.listen();

  pixels_FS.begin(); // This initializes the NeoPixel library.
  pixels_forest.begin();
  pixels_climate.begin();
  pixels_playback.begin();
  pixels_whiteStrip.begin();
  turnOnLedBar();

}

void loop() {

  readButtons();
  updateLeds();
}


void readButtons() {
  for (int i = 0; i < 4; i++) {
    int buttonState = digitalRead(buttonPins[i]);
    int lastButtonState = lastButtonStates[i];

    if (buttonState && !lastButtonState) {
      activeMode = i;
      //      Serial.print("mode ");
      //      Serial.print( activeMode);
      //      Serial.println(" active");

      Serial.println(activeMode);
    }
    lastButtonStates[i] = buttonState;

  }
}

void updateLeds() {
  for (int i = 0; i < 4; i++) {
    if (activeMode == i) {
      setledStripColor(i, 255, 255, 255);
    } else {
      setledStripColor(i, 0, 0, 0);
    }
  }
}


void setledStripColor(int ledStripIndex, int r, int g, int b) {

  switch (ledStripIndex) {

    case 0:
      for (int i = 0; i < ledNumbers[ledStripIndex]; i++) {
        pixels_FS.setPixelColor(i, pixels_FS.Color(r, g, b));
      }
      pixels_FS.show();
      break;
    case 1:
      for (int i = 0; i < ledNumbers[ledStripIndex]; i++) {
        pixels_forest.setPixelColor(i, pixels_forest.Color(r, g, b));
      }
      pixels_forest.show();
      break;
    case 2:
      for (int i = 0; i < ledNumbers[ledStripIndex]; i++) {
        pixels_climate.setPixelColor(i, pixels_climate.Color(r, g, b));
      }
      pixels_climate.show();
      break;
    case 3:
      for (int i = 0; i < ledNumbers[ledStripIndex]; i++) {
        pixels_playback.setPixelColor(i, pixels_playback.Color(r, g, b));
      }
      pixels_playback.show();

      break;
  }
}

void turnOnLedBar() {
  for (int i = 0; i < 70; i++) {
    pixels_whiteStrip.setPixelColor(i, pixels_whiteStrip.Color(100, 100, 100));
  }
  pixels_whiteStrip.show();
}


////here 3 functions to send/receive msg from/to the broker
//void subscribeToChannel(String topic) {
//  lolin.print("subscribe-");
//  lolin.print("topic");
//  lolin.print('\n');
//}
//
//void sendMsg(String topic, String msg) {
//  lolin.print(topic);
//  lolin.print("-");
//  lolin.print(msg);
//  lolin.print('\n');
//}

//void checkIncomingmsg() {
//
//  if (lolin.available() > 0) {
//
//    Serial.println("gotdata");
//    String command = lolin.readStringUntil('\n');
//    Serial.println(command);
//
////    int separatorIndex = command.indexOf('-');
////
////    if (separatorIndex > 1) {
////      String arg1 = command.substring(0, separatorIndex);
////      String arg2 = command.substring(separatorIndex + 1, command.length());
////
////      Serial.println ("--------");
////      Serial.println ("received msg:");
////      Serial.println (arg1);
////      Serial.println (arg2);
////      Serial.println ("--------");
////    }
//  }
//}





