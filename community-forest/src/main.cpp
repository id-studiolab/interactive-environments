#include <Arduino.h>
#include "ForestController.h"
#include <MQTT.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const int id = 9;
const int leds = 6;

ForestController *controller;
WiFiClientSecure net;

MQTTClient client;
void connect();
void onMessage(String &topic, String &payload);
String splitString(String data, char separator, int index);

void setup()
{
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    controller = new ForestController(id, leds);

    client.begin("broker.shiftr.io", 8883, net);
    client.onMessage(onMessage);
    delay(2000);
    client.connect(String(id).c_str(), MQTT_USERNAME, MQTT_PASSWORD);
    client.subscribe("/forest/time");
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
        client.connect(String(id).c_str(), MQTT_USERNAME, MQTT_PASSWORD);
        client.subscribe("/forest/time");
        client.subscribe("/forest/led");
        client.subscribe("/forest/moisture");

        if (client.connected())
        {
            Serial.println("mqtt connected");
        }
    }
}

void onMessage(String &topic, String &payload)
{
    Serial.println("incoming: " + topic + " - " + payload);

    if (topic == "/forest/led")
    {
        int id = splitString(payload, ' ', 0).toInt();
        if (splitString(payload, ' ', 0).toInt() == id)
        {
            double hue = splitString(payload, ' ', 1).toFloat();
            Serial.print("starting led with new hue ");
            Serial.println(hue);
            controller->startLED(hue);
        }
    }
    else if (topic == "/forest/time")
    {
        controller->setCycleTime(payload.toInt());
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