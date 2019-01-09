#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>

const char ssid[] = "iot-net";
const char pass[] = "interactive";

SoftwareSerial mySerial(5, 6);

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
  while (!client.connect("CoZone-Data2", "a0e78aaf", "2626bb47aaf15e04")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
}

void setup() {
  Serial.begin(19200);
  mySerial.begin(9600);
  WiFi.begin(ssid, pass);

  client.begin("broker.shiftr.io", net);

  connect();
  delay(1000);
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }
  
  if (mySerial.available()) {
    String data = mySerial.readString();
    data.remove(1);

    if (data == "0" or data == "1" or data == "2" or data == "3" or data == "4" or data == "5" or data == "6") {
      client.publish("/cozone/busyness", data);
    }
  }
}
