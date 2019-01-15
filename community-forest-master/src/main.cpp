#include <Arduino.h>
#include <MQTT.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define MQTT_NAME "ForestMaster"
#define MQTT_USERNAME "a0e78aaf"
#define MQTT_PASSWORD "2626bb47aaf15e04"

WiFiClientSecure net;
MQTTClient client;
void connect();
void onMessage(String &topic, String &payload);
double getNewHue(double current);
void setHue();
void sendHue();
void sendLED(int module, double brightnessIncrease, double hueIncrease, double hue);
void sendTotalHumidity();
String splitString(String data, char separator, int index);

int totalModules = 10;
unsigned long timer = 0;
unsigned long hueTimer = 0;
unsigned long hueInterval = 120000;
int currentModule = -1;
int oldHue = 120.0;
int currentHue = currentHue;

bool plantStates[30];
unsigned long humidityTimer = 0;
unsigned long humidityInterval = 300000;

int cycle = 600000; //10 minutes
// cycle = 150000;
unsigned int timerInterval() { return cycle / totalModules; }

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
  client.subscribe("/forest/humidity");
  connect();
  delay(5000);
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

  if (millis() - hueTimer >= hueInterval)
  {
    hueTimer = millis();
    sendHue();
  }

  if (millis() - humidityTimer >= humidityInterval)
  {
    humidityTimer = millis();
    sendTotalHumidity();
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
    client.subscribe("/forest/humidity");

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
  oldHue = currentHue;
  currentHue = getNewHue(currentHue);
  sendHue();
}

void sendHue()
{
  Serial.println("sending " + String(map(currentHue, 0, 360, 0, 255)) + ' ' + "to forest/hue");
  client.publish("forest/color/h", String(currentHue));
}

void sendLED(int module, double brightnessIncrease, double hueIncrease, double hue)
{
  String msg = String(module);
  msg += " ";
  msg += String(hue);
  msg += " ";
  msg += String(timerInterval());
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
  else if (topic == "/forest/humidity")
  {
    int id = splitString(payload, ' ', 0).toInt();
    bool watered = splitString(payload, ' ', 1) == "1";
    plantStates[id] = watered;
  }
}

void sendTotalHumidity()
{
  int total = 0;
  for (int i = 0; i < totalModules; i++)
  {
    if (plantStates[i])
      total++;
  }
  int mapped = map(total, 0, totalModules, 0, 100);
  client.publish("forest/totalHumidity", String(mapped));
}

String splitString(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
