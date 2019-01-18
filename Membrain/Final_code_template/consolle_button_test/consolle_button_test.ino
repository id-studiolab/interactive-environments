#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11

int buttonPins [4] = { 4, 3, 2, 19 };
int ledNumbers [4] = { 6, 6, 10, 12 };
int ledPins [4] = { 9, 8, 7, 6 };

Adafruit_NeoPixel pixels_FS = Adafruit_NeoPixel(ledNumbers[0], ledPins [0], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_forest = Adafruit_NeoPixel(ledNumbers[1], ledPins [1], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_climate = Adafruit_NeoPixel(ledNumbers[2], ledPins [2], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_playback = Adafruit_NeoPixel(ledNumbers[3], ledPins [3], NEO_GRB + NEO_KHZ800);

SoftwareSerial lolin (rxPin, txPin);

unsigned long lastActivityCheck = 0;
unsigned long lastSent = 0;

int activeMode = 0;

long intervalBetweenEachMode = 1000 * 60 * 2; //2 minutes
long lastTimeUpdated = 0;

void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(9600);
  lolin.begin(9600);
  //lolin.listen();

  pixels_FS.begin(); // This initializes the NeoPixel library.
  pixels_forest.begin();
  pixels_climate.begin();
  pixels_playback.begin();

  subscribeToChannel("/membrane/mode");

}

void loop() {

  readButtons();
  updateLeds();
  checkIncomingmsg();
}


void readButtons() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(buttonPins[i]) == HIGH && activeMode != i) {
      activeMode = i;
      Serial.print("mode ");
      Serial.print( i);
      Serial.println(" active");

      sendMsg("/membrane/mode", String(activeMode));

    }
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


//here 3 functions to send/receive msg from/to the broker
void subscribeToChannel(String topic) {
  lolin.print("subscribe-");
  lolin.print("topic");
  lolin.print('\n');
}

void sendMsg(String topic, String msg) {
  lolin.print(topic);
  lolin.print("-");
  lolin.print(msg);
  lolin.print('\n');
}

void checkIncomingmsg() {

  if (lolin.available() > 0) {

    Serial.println("gotdata");
    String command = lolin.readStringUntil('\n');
    Serial.println(command);

//    int separatorIndex = command.indexOf('-');
//
//    if (separatorIndex > 1) {
//      String arg1 = command.substring(0, separatorIndex);
//      String arg2 = command.substring(separatorIndex + 1, command.length());
//
//      Serial.println ("--------");
//      Serial.println ("received msg:");
//      Serial.println (arg1);
//      Serial.println (arg2);
//      Serial.println ("--------");
//    }
  }
}





