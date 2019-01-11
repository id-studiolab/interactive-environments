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
void setHue();
void sendLED(int module, double brightnessIncrease, double hueIncrease, double hue);

int totalModules = 10;
unsigned long timer = 0;
int currentModule = -1;
int currentHue = 120.0;

int cycle = 600000; //10 minutes
// int cycle = 150000;
unsigned int timerInterval() { return cycle / totalModules; }
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
  client.subscribe("/forest/total");
  connect();
  delay(10000);
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

  if (millis() - timer >= timerInterval())
  {
    timer = millis();
    connect();

    currentModule++;
    if (currentModule >= totalModules)
    {
      currentModule = 0;
      setHue();
    }
    sendLED(currentModule, 0, 0, currentHue);
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
    client.subscribe("/forest/total");

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
}

void sendLED(int module, double brightnessIncrease, double hueIncrease, double hue)
{
  String msg = String(module);
  msg += " ";
  msg += String(hue);
  Serial.println("sending: " + msg + " to /forest/led");
  client.publish("/forest/led", msg);
}

void onMessage(String &topic, String &payload)
{
  Serial.println("incoming: " + topic + " - " + payload);

  if (topic == "/forest/time")
  {
    cycle = payload.toInt();
  }
  else if (topic == "/forest/total")
  {
    totalModules = payload.toInt();
  }
}
