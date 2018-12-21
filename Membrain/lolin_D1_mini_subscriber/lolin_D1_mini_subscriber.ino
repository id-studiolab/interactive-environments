#include <ESP8266WiFi.h>
#include <MQTT.h>

const int noPetisomes = 50;
const char ssid[] = "iot-net";
const char pass[] = "interactive";

bool stringComplete = false;

int noPetisomeFocussed = 0;
int noPetisomeActive = 0;

WiFiClient net;
MQTTClient client;

unsigned long updateListTime = 0;
unsigned long updateActivenessTime = 0;

struct petisome {
  bool focussed = false;
  unsigned long lastUpdate = 0;
  bool active = false;
} petisomes [noPetisomes];

void updateList() {
  noPetisomeActive = 0;
  noPetisomeFocussed = 0;
  for (int i = 0; i < noPetisomes; i++) {
    if (petisomes[i].active) {
      noPetisomeActive ++;
      if (petisomes[i].focussed) {
        noPetisomeFocussed ++;
      }
    }
  }
}

void updateActiveness() {
  for (int i = 0; i < noPetisomes; i++) {
    if (petisomes[i].active) {
      if (millis() - petisomes[i].lastUpdate > 60000) {
        petisomes[i].active = false;
      }
    }
  }
}

void connect() {
  //Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    delay(1000);
  }

  //Serial.print("\nconnecting...");
  while (!client.connect("Nieuwe Chip", "sharing", "caring")) {
    //Serial.print(".");
    delay(1000);
  }

  //Serial.println("\nconnected!");

  client.subscribe("/deskmateimitation/availability/#");
}


void messageReceived(String &topic, String &payload) {
  int peti = (topic.substring(32).toInt());
  //Serial.println(String(peti) + ":" + payload + "!");
  petisomes[peti].lastUpdate = millis();
  petisomes[peti].active = true;
  if (payload.equals("focussed")) {
    petisomes[peti].focussed = true;
  } else {
    petisomes[peti].focussed = false;
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, INPUT_PULLUP);

  client.begin("192.168.1.23", net);
  client.onMessage(messageReceived);

  connect();
  delay(1000);
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }

  if (millis() - updateListTime > 2000) {
    updateActiveness();
    updateList();
    Serial.print("pa:" + String(noPetisomeActive) + "!");
    Serial.print("pf:" + String(noPetisomeFocussed) + "!");
    updateListTime = millis();
  }


  client.publish("/test", "pa:"+String(noPetisomeActive)+" pf:"+ String(noPetisomeFocussed));
  delay(2000);

}
