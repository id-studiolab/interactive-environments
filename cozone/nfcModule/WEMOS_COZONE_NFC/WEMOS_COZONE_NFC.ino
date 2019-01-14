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

char temp = '0';
int cur = 0;

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
    Serial.println("Reconnecting");
  }
  
  if (mySerial.available()) {
    char c = mySerial.read();
    if (c == '\n' or c == '\r') {
      if (millis() - cur > 1000) {
        client.publish("/cozone/busyness", String(temp));
        cur = millis();
      }
    } else {
      temp = c;
    }
  }
}
