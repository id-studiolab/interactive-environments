#include <Arduino.h>
#include "ForestController.h"
#include <MQTT.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const int id = 0;
const int amountLeds = 6;
ForestController *controller;
WiFiClientSecure net;

MQTTClient client;
void connect();
void onMessage(String &topic, String &payload);

void setup()
{
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    controller = new ForestController(id, amountLeds);

    client.begin("broker.shiftr.io", 8883, net);
    client.onMessage(onMessage);
    delay(2000);
    client.connect(String(id).c_str(), MQTT_USERNAME, MQTT_PASSWORD);
    client.subscribe("/forest/time");
    client.subscribe("/forest/led");
    client.subscribe("/forest/hue");
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
        controller->setLED(amountLeds, 255, 0);
    }
    else if (!client.connected())
    {
        Serial.println("No mqtt connection");

        controller->setLED(amountLeds, 255, 240);
        client.connect(String(id).c_str(), MQTT_USERNAME, MQTT_PASSWORD);
        client.subscribe("/forest/time");
        client.subscribe("/forest/led");
        client.subscribe("/forest/hue");
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

    if (topic == "/forest/hue")
    {
        controller->setHue(payload.toFloat());
    }
    else if (topic == "/forest/led")
    {
        if (payload.toInt() == id)
        {
            Serial.println("starting led");
            controller->startLED();
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
