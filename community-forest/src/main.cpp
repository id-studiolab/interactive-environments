#include <Arduino.h>
#include "ForestController.h"
#include <MQTT.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const int id = CONTROLLER_ID;
const int leds = AMOUNT_LEDS;

ForestController *controller;
WiFiClient net;

MQTTClient client;
void connect();
void onMessage(String &topic, String &payload);
String splitString(String data, char separator, int index);
void sendMoistureState(bool moistured);

bool plantWatered = false;
unsigned long plantWateredTimer = 0;
unsigned long plantWateredInterval = 1000;
const int waterThreshold = 400;

void setup()
{
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    controller = new ForestController();

    client.begin("192.168.1.23", net);
    client.onMessage(onMessage);
    delay(2000);
    client.connect(("Forest" + String(id)).c_str(), MQTT_USERNAME, MQTT_PASSWORD);
    client.subscribe("/forest/led");
    client.subscribe("/forest/moisture");
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
    controller->loop();

    if (millis() - plantWateredTimer >= plantWateredInterval)
    {
        int moistureValue = controller->getMoistureSensorValue();
        if (moistureValue < waterThreshold && plantWatered)
        {
            plantWatered = false;
            sendMoistureState(false);
        }
        else if (moistureValue > waterThreshold && !plantWatered)
        {
            plantWatered = true;
            sendMoistureState(true);
        }
    }
    delay(10);
}

void connect()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        // WiFi.begin(ssid, password);
        Serial.println("No Wifi connection");
        controller->setLED(leds, 255, 0);
    }
    else if (!client.connected())
    {
        Serial.println("No mqtt connection");

        controller->setLED(leds, 255, 240);
        client.connect(("Forest" + String(id)).c_str(), MQTT_USERNAME, MQTT_PASSWORD);
        client.subscribe("/forest/led");
        client.subscribe("/forest/moisture");

        if (client.connected())
        {
            Serial.println("mqtt connected");
        }
    }
    delay(200);
}

void onMessage(String &topic, String &payload)
{
    Serial.println("incoming: " + topic + " - " + payload);

    if (topic == "/forest/led")
    {
        if (splitString(payload, ' ', 0).toInt() == id)
        {
            double hue = splitString(payload, ' ', 1).toFloat();
            double timeTaken = splitString(payload, ' ', 2).toFloat();
            Serial.print("starting led with new hue ");
            Serial.print(hue);
            Serial.print(" and time ");
            Serial.println(timeTaken);
            controller->startLED(hue, timeTaken);
        }
    }
    else if (topic == "/forest/moisture")
    {
        if (payload.toInt() == 1)
        {
            controller->enableMoisture(false);
        }
        else
        {
            controller->enableMoisture(true);
        }
    }
}

void sendMoistureState(bool moistured)
{
    if (moistured)
    {
        client.publish("forest/watered", String(1));
    }
    client.publish("forest/" + String(id) + "/humidity", moistured ? "1" : "0");
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