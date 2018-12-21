#include <ESP8266WiFi.h>
#include <MQTT.h>

const char ssid[] = "iot-net";
const char pass[] = "interactive";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;
String message = "";
char delimitKV = ':';
char delimitEnd = '!';

void connect() {
  //Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    delay(1000);
  }

  //Serial.print("\nconnecting...");
  while (!client.connect("Deskmate Imitator", "sharing", "caring")) {
    //Serial.print(".");
    delay(1000);
  }

  //Serial.println("\nconnected!");

  client.subscribe("/deskmateimitation/#");
}


void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  client.begin("192.168.1.23", net);

  connect();
  delay(1000);
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }

  if (Serial.available()) {
    char r = Serial.read();
    if (r == delimitEnd) {
      int indexDelimitKV = message.indexOf(delimitKV);
      String pNoS = message.substring(0, indexDelimitKV);

      int pNo = -1;
      if (!isnan(pNoS.toInt())) {
        pNo = pNoS.toInt();
      }
      String value = message.substring(indexDelimitKV + 1);

      switch (pNo) {
        case 0:
          client.publish("/deskmateimitation/availability/0", value);
          break;
        case 1:
          client.publish("/deskmateimitation/availability/1", value);
          break;
        case 2:
          client.publish("/deskmateimitation/availability/2", value);
          break;
        case 3:
          client.publish("/deskmateimitation/availability/3", value);
          break;
        case 4:
          client.publish("/deskmateimitation/availability/4", value);
          break;
        case -1:
          //client.publish("/deskmateimitation/availability/nan", value);
          break;
      }
      message = "";
    } else {
      message += r;

    }
  }

}
