//#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>

const char ssid[] = "Science-Centre-EVENT";
const char pass[] = "ScienceCentre";

String inputString = "";
bool stringComplete = false;

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("CoZone-Data", "a0e78aaf", "2626bb47aaf15e04")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(5, INPUT_PULLUP);

  client.begin("broker.shiftr.io", net);

  inputString.reserve(200);

  connect();
  delay(1000);
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }

  //client.publish("/test", "test");
  delay(2000);

}
