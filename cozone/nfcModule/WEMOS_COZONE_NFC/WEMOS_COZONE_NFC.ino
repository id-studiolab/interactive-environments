#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>

const char ssid[] = "iot-net";
const char pass[] = "interactive";

SoftwareSerial mySerial(5, 4);

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  Serial.print("\nconnecting...");
  if (!client.connect("CoZone-Data2", "a0e78aaf", "2626bb47aaf15e04")) {
    delay(3000);
    return;
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
    Serial.println("Reconnecting");
  }
  
  if (mySerial.available()) {
    String data = mySerial.readString();
    data.remove(1);
    Serial.println("I received " + data);

    if (data == "0" or data == "1" or data == "2" or data == "3" or data == "4" or data == "5" or data == "6") {
      client.publish("/cozone/busyness", data);
    }
  }
}
