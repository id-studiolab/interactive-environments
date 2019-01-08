#include <Arduino.h>
#include <MQTT.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Ticker.h>

#define MQTT_NAME "Master"
#define MQTT_USERNAME "a0e78aaf"
#define MQTT_PASSWORD "2626bb47aaf15e04"

WiFiClientSecure net;
MQTTClient client;
void connect();
void onMessage(String &topic, String &payload);
double getNewHue(double current);
void sendHue();
void setHue();
void sendLED(int module, double brightnessIncrease, double hueIncrease, double nextHue);

const int totalModules = 28;
unsigned long timer = 0;
int currentModule = 0;
int currentHue = 120.0;

// int shortCycle = 600000; //10 minutes
int shortCycle = 150000;
unsigned int timerInterval = shortCycle / totalModules;
Ticker hueSend;

void setup()
{
  Serial.begin(115200);
  WiFi.begin("iot-net", "interactive");

  client.begin("broker.shiftr.io", 8883, net);
  client.onMessage(onMessage);
  delay(2000);
  client.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD);
  client.subscribe("/forest/time");
  client.subscribe("/forest/led");
  client.subscribe("/forest/nextHue");
  connect();
  Serial.println("Setup Done");
}

void loop()
{
  if (client.connected())
  {
    client.loop();
  }
  else
  {
    connect();
  }

  if (millis() - timer >= timerInterval)
  {
    timer = millis();
    connect();

    currentModule++;
    sendLED(currentModule, 0, 0, currentHue);
    if (currentModule == totalModules)
    {
      currentModule = 0;
      setHue();
    }
    Serial.println("currentStrip = " + String(currentModule) + ", next hue = " + currentHue);
  }

  delay(10);
}

void connect()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    // WiFi.begin(ssid, password);
    Serial.println("No Wifi connection");
  }
  else if (!client.connected())
  {
    Serial.println("No mqtt connection");

    client.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD);
    client.subscribe("/forest/time");
    client.subscribe("/forest/led");
    client.subscribe("/forest/nextHue");

    if (client.connected())
    {
      Serial.println("mqtt connected");
    }
  }
}

double getNewHue(double current)
{
  return fmod(current + (random(0, 10) > 5 ? 1.0 : -1.0) * random(40, 140) + 360.0, 360.0);
}

void setHue()
{
  currentHue = getNewHue(currentHue);
  if (client.connected())
  {
    sendHue();
  }
  else
  {
    hueSend.attach(10, sendHue);
  }
}

void sendLED(int module, double brightnessIncrease, double hueIncrease, double nextHue)
{
  String msg = String(module);
  Serial.println("sending: " + msg + " to /forest/led");
  client.publish("/forest/led", msg);
}

void sendHue()
{
  if (client.connected())
  {
    hueSend.detach();
    Serial.println("sending " + String(currentHue) + ' ' + "to forest/hue");
    client.publish("forest/hue", String(currentHue));
  }
  else
  {
    connect();
  }
}

void onMessage(String &topic, String &payload)
{
  Serial.println("incoming: " + topic + " - " + payload);

  if (topic == "/forest/hue")
  {
  }
}
