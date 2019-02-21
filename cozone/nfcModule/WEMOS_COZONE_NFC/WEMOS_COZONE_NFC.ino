#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>

// ssid & password for WiFi
const char ssid[] = "iot-net";
const char pass[] = "interactive";

// Initialize SoftwareSerial for receiving from UNO board
SoftwareSerial mySerial(5, 4);

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Log in to MQTT Broker
  Serial.print("\nconnecting...");
  while (!client.connect("CoZone-Data", "sharing", "caring")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
}

void setup() {
  Serial.begin(19200);
  mySerial.begin(9600);
  WiFi.begin(ssid, pass);

  // Connect to broker website
  client.begin("192.168.1.23", net);

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
    // Read incoming data from UNO board
    char c = mySerial.read();
    if (c == '\n' or c == '\r') {
      // Send data to the MQTT service every other second
      if (millis() - cur > 1000) {
        client.publish("/cozone/busyness", String(temp));
        cur = millis();
      }
    } else {
      temp = c;
    }
  }
}
