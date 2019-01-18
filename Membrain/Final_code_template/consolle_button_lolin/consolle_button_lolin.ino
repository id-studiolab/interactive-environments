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

String channelsToSubscribeTo[10] = {"/membrane/mode", "", "", "", "", "", "", "", "", "",};

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

  subscribeToTopics();
}


void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
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
  parseCommandFromSerial();
}


void parseCommandFromSerial() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');

    int separatorIndex = command.indexOf('-');

    String arg1 = command.substring(0, separatorIndex);
    String arg2 = command.substring(separatorIndex + 1, command.length());

    if (arg1 == "subscribe") {
      addTopicToSubscribeTo(arg2);
    } else {
      client.publish(arg1, arg2);
    }
  }
}



void messageReceived(String &topic, String &payload) {
  Serial.println(topic + "-" + payload);
}


//doing this from the function does not work somehow
void addTopicToSubscribeTo(String topic) {
  for (int i = 0; i < 10; i++) {
    if (channelsToSubscribeTo[i] == "") {
      channelsToSubscribeTo[i] = topic;

      for (int j = 0; j < i + 1; j++) {
        Serial.print("_");
        Serial.print(channelsToSubscribeTo[j]);
        Serial.println("_");
      }
      break;

    }
  }
  client.disconnect();
  connect();
}

void subscribeToTopics() {
  for (int i = 0; i < 10; i++) {
    if (channelsToSubscribeTo[i] != "") {
      client.subscribe(channelsToSubscribeTo[i]);
      
      Serial.print("subscribed To:");
      Serial.print(channelsToSubscribeTo[i]);
      Serial.println();
    } else {
      Serial.println("list done");
      break;
    }
  }
}


