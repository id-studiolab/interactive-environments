#include "ForestController.h"

ForestController::ForestController(int identifier, int lPerStrip)
{
    id = identifier;
    mqttUsername = String(identifier).c_str();
    amountLeds = lPerStrip;
    FastLED.addLeds<NEOPIXEL, LEDPIN>(strip, lPerStrip);
    setLED(lPerStrip, minBrightness, currentHue);
    FastLED.show();

    pinMode(MOISTURE_PIN, INPUT);

    // setHue();
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    delay(2000);
    client.begin("broker.shiftr.io", 8883, net);
    client.onMessage([this](String &topic, String &payload) -> void {
        ForestController::onMessage(topic, payload);
    });
    delay(2000);
    client.connect(mqttUsername.c_str(), MQTT_USERNAME, MQTT_PASSWORD);
    client.subscribe("/forest/time");
    client.subscribe("/forest/led");
    client.subscribe("/forest/nextHue");
    connect();
    Serial.println("init completed");
}

void ForestController::setLED(int amount, int brightness, double hue)
{
    int mappedHue = map(hue, 0, 360, 0, 255);
    for (int i = 0; i < amountLeds; i++)
    {
        strip[i].setHSV(mappedHue, 255, brightness);
    }
}

void ForestController::connect()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        // WiFi.begin(ssid, password);
        Serial.println("No Wifi connection");
    }
    else if (!client.connected())
    {
        client.connect(mqttUsername.c_str(), MQTT_USERNAME, MQTT_PASSWORD);
        client.subscribe("/forest/time");
        client.subscribe("/forest/led");
        client.subscribe("/forest/nextHue");

        Serial.println("No mqtt connection");
    }
    brightnessTimer = millis();
    timer = millis();
}

int ForestController::getAmountOfLeds()
{
    int value = analogRead(MOISTURE_PIN);
    int amount;
    if (value > 2800)
    {
        amount = map(value, 2800, 4096, amountLeds, 2);
    }
    else
    {
        amount = map(value, 0, 2800, 2, amountLeds);
    }

    return constrain(amount, 2, amountLeds);
}

void ForestController::loop()
{
    if (client.connected())
    {
        client.loop();
    }
    else
    {
        connect();
    }

    if (millis() - brightnessTimer >= brightnessInterval)
    {
        brightnessTimer = millis();
        if (activatedBrightness <= targetBrightness)
            activatedBrightness += brightnessIncrease();
        if (activatedBrightness >= targetBrightness)
            activatedBrightness -= brightnessIncrease();

        if (activatedBrightness >= maxBrightness - 5)
            targetBrightness = minBrightness;

        if (activatedBrightness < 0)
            activatedBrightness = 0;
        if (activatedBrightness > 255)
            activatedBrightness = 255;

        if (abs(nextHue - activatedHue) > abs(hueIncrease * 2.0))
        {
            activatedHue = fmod(activatedHue + hueIncrease + 360.0, 360.0);
        }

        setLED(getAmountOfLeds(), minBrightness, activatedHue);

        FastLED.show();
    }

    delay(5);
}

void ForestController::onMessage(String &topic, String &payload)
{
    Serial.println("incoming: " + topic + " - " + payload);

    if (topic == "/forest/nextHue")
    {
        currentHue = nextHue;
        nextHue = payload.toFloat();
    }
    else if (topic == "/forest/led")
    {
        if (payload.toInt() == id)
        {
            Serial.println("starting led");
            startLED();
        }
    }
    else if (topic == "/forest/time")
    {
        if (payload == "slow")
        {
            shortCycle = 600000;
        }
        else
        {
            shortCycle = 150000;
        }
        timerInterval = shortCycle / totalStrips;
    }
}

void ForestController::startLED()
{
    hueIncrease = 2.0 * calculateHsvIncrease(currentHue, nextHue, brightnessInterval, timerInterval);
    activatedHue = currentHue;
    activatedBrightness = minBrightness;
    targetBrightness = maxBrightness;
}