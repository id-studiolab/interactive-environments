#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>

const char ssid[] = "Science-Centre-EVENT";
const char pass[] = "ScienceCentre";

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
  while (!client.connect("CoZone-Data", "a0e78aaf", "2626bb47aaf15e04")) {
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

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }
  
  if (mySerial.available()) {
    String data = mySerial.readString();
    String dataTemp = getValue(data, '\n', 0);
    String dataHumi = getValue(data, '\n', 1);
    String dataCarb = getValue(data, '\n', 2);

    dataTemp.remove(4);
    dataHumi.remove(4);
    dataCarb.remove(4);

    if (dataCarb.endsWith("C")) {
      dataCarb.remove(3);
    }

    client.publish("/cozone/temperature", dataTemp);
    client.publish("/cozone/humidity", dataHumi);
    client.publish("/cozone/co2", dataCarb);
  }
}
