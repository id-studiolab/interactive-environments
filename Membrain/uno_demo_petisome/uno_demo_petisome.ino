#include <ChainableLED.h>
#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11
#define NUM_LEDS  5
const int noPetisomes = 5;

ChainableLED leds(7, 8, NUM_LEDS);
SoftwareSerial mySerial (rxPin, txPin);


unsigned long lastActivityCheck = 0;
unsigned long lastSent = 0;

int buttonPins [noPetisomes] = { 2, 3, 4, 5, 6 };


struct petisome {
  bool focussed;
  unsigned long lastHello;
  bool active;
} petisomes [noPetisomes];

void updateLeds() {
  Serial.println("updating Leds");
  for (int i = 0; i < noPetisomes; i++) {
    if (petisomes[i].active) {
      if (petisomes[i].focussed) {
        leds.setColorRGB(i, 0, 120, 160);
      } else {
        leds.setColorRGB(i, 0, 206, 10);
      }
    } else {
      leds.setColorRGB(i, 0, 0, 0);
    }
  }
}

void updateActiveness() {
  for (int i = 0; i < noPetisomes; i++) {
    if (petisomes[i].active) {
      if (millis() - petisomes[i].lastHello > 60000) {
        petisomes[i].active = false;
      }
    }
  }
}

void sendState(int peti) {
  mySerial.print(String(peti));
  mySerial.print(":");
  if (petisomes[peti].focussed) {
    mySerial.print("focussed");
  } else {
    mySerial.print("social");
  }
  mySerial.print("!");
  delay(100);
}
void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  mySerial.listen();

  leds.init();

  for (int i = 0; i < noPetisomes; i++) {
    leds.setColorRGB(i, 0, 0, 0);
    pinMode(buttonPins[i], INPUT);
    petisomes[i].active = false;
    petisomes[i].lastHello = millis();
    petisomes[i].focussed = false;
  }

}

void loop() {
  for (int i = 0; i < noPetisomes; i++) {
    if (digitalRead(buttonPins[i]) == HIGH) {
      petisomes[i].focussed = !petisomes[i].focussed;
      sendState(i);
      petisomes[i].active = true;
      petisomes[i].lastHello = millis();
      delay(150);
    }
  }

  updateLeds();

  if (millis() - lastActivityCheck > 1000) {
    updateActiveness();
    lastActivityCheck = millis();
  }

  if (millis() - lastSent > 10000) {
    for (int i = 0; i < noPetisomes; i++) {
      if (petisomes[i].active) {
        sendState(i);
      }
    }
    lastSent = millis();
  }
}
