#include <ESP8266WiFi.h>
#include <MQTT.h>

#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 70

#define DATA_PIN 5

// Define the array of leds
CRGB leds[NUM_LEDS];

const char ssid[] = "iot-net";
const char pass[] = "interactive";

WiFiClient net;
MQTTClient client;

String topic = "/membrane/mode";

void connect() {
  //Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  //Serial.print("\nconnecting...");
  while (!client.connect("Deskmate Imitator", "sharing", "caring")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  //client.subscribe("/hello");

  client.subscribe(topic);

}


void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, pass);
  client.begin("192.168.1.23", net);
  client.onMessage(messageReceived);

  connect();
  delay(1000);

  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);

}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }
}


void messageReceived(String &topic, String &payload) {
  Serial.print(topic);
  Serial.print("-");
  Serial.print(payload);
  Serial.print("\n");


  
  switch (payload.toInt()) {
    case 0:
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].setRGB( 255, 0, 0);
      }
      break;
    case 1:
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].setRGB( 0, 255, 0);
      }
      break;
    case 2:
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].setRGB( 0, 0, 255 );
      }
      break;
    case 3:
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].setRGB( 255, 255,0 );
      }
      break;
  }
    FastLED.show();

}








