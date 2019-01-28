/*
This code allows an arduino wemos to function as a data bridge between the MeetNow and the DailyMeet.
For the communication with the arduino Mega it uses a Software Serial communication.
For the communication with the other device it uses a MQTT server.
*/

// ---------------
// -- Libraries --
// ---------------

#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D6, D7); //RX, TX
WiFiClient net;
MQTTClient client;


// ---------------
// -- Variables --
// ---------------

const char ssid[] = "iot-net";
const char pass[] = "interactive";

long startbookings[24];
long endbookings[24];
int NUMevents = 0;

// ------------------
// -- Setup & Loop --
// ------------------

void setup() {
  Serial.begin(19200);
  mySerial.begin(9600);
  WiFi.begin(ssid, pass);
  client.begin("192.168.1.23", net);
  client.onMessage(messageReceived);
  Serial.println("connecting");
  connect();
  delay(1000);
}

void loop() {
  client.loop();
  delay(10);
  if (!client.connected()) {
    connect();
  }
  if (mySerial.available() > 0) {
    String nextmeeting = mySerial.readStringUntil('\n');
    Serial.println(nextmeeting);
    client.publish("/meet/now/", nextmeeting);
  }
}

// ---------------
// -- Functions --
// ---------------

void connect() {
  Serial.println("getting wifi ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  Serial.print("connecting");
  while (!client.connect("MeetNow", "sharing", "caring")) {
    Serial.print(".");
    delay(1000);
  }
  client.subscribe("/meet/daily/");

  Serial.println("connected!");
}

void messageReceived(String &topic, String &payload) {
  Serial.println(payload);
  mySerial.write(payload.c_str());
  mySerial.write('\n');
}






