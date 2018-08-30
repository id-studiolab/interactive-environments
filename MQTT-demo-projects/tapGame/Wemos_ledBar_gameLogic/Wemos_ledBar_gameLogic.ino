#include <ESP8266WiFi.h>
#include <MQTTClient.h>

const char *ssid = "gino";
const char *pass = "ginogino";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;


#include <Adafruit_NeoPixel.h>
#define PIN            D4
#define NUMPIXELS      20
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

float nRedPixel = 10;


void connect(); // <- predefine connect() for setup()

void setup() {


  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  client.begin("broker.shiftr.io", net);
  connect();
  pixels.begin(); // This initializes the NeoPixel library.



}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nconnecting...");
  while (!client.connect("d1_tapGame_Master", "a0e78aaf", "2626bb47aaf15e04")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/tapGame/*");
}

void loop() {

  if (!client.connected()) {
    connect();
  }

  client.loop();
  delay(10); // <- fixes some issues with WiFi stability

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {

    Serial.println("timerCount");
    if (nRedPixel>10){
      nRedPixel-=.2;
    }else if (nRedPixel>10){
      nRedPixel+=.2;
    }
    client.publish("/tapGame/redProgress", String(nRedPixel / 20, DEC));

    lastMillis=millis();

  }

  updateLed();
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();

  boolean needToSendoutNewStats = false;
  if (topic == "/tapGame/team0") {
    Serial.println("increase red score");
    nRedPixel += .2;
    needToSendoutNewStats = true;
  } else if (topic == "/tapGame/team1") {
    nRedPixel -= .2;
    needToSendoutNewStats = true;
  }

  if (needToSendoutNewStats) {

    if (nRedPixel > 19) {
      client.publish("/tapGame/victory", "red");
      redVictory();
    } else if (nRedPixel < 1) {
      client.publish("/tapGame/victory", "blue");
      blueVictory();
    }

    client.publish("/tapGame/redProgress", String(nRedPixel / 20, DEC));
  }
}

void updateLed() {
  for (int i = 0; i < NUMPIXELS; i++) {

    if (i < nRedPixel ) {
      pixels.setPixelColor(i, pixels.Color(150, 0, 0)); // Moderately bright green color.

    } else {
      pixels.setPixelColor(i, pixels.Color(0, 0, 150)); // Moderately bright green color.
    }

  }
  pixels.show(); // This sends the updated pixel color to the hardware.
}


void blueVictory() {
  for (int k = 0; k < 3; k++) {
    for (int i = 0; i < NUMPIXELS; i++) {
      for (int j = 0; j < NUMPIXELS; j++) {
        if (j < i) {
          pixels.setPixelColor(j, pixels.Color(0, 0, 150));
        } else {
          pixels.setPixelColor(j, pixels.Color(0, 0, 0));
        }
        pixels.show();
        delay(2);
      }
    }
  }
    resetGame();

}

void redVictory() {
  for (int k = 0; k < 3; k++) {
    for (int i = 0; i < NUMPIXELS; i++) {
      for (int j = 0; j < NUMPIXELS; j++) {
        if (j < i) {
          pixels.setPixelColor(j, pixels.Color(150, 0, 0));
        } else {
          pixels.setPixelColor(j, pixels.Color(0, 0, 0));
        }
        pixels.show();
        delay(2);
      }
    }
  }
  resetGame();
}

void resetGame() {
  nRedPixel=10;
  client.publish("/tapGame/redProgress", String(nRedPixel / 20, DEC));
}


