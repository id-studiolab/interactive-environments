#include <Arduino.h>

#include <FastLED.h>
#include <math.h>

#include <stdexcept>
#include <vector>
#include <cstring>

#ifndef FORESTCONTROLLER_H
#define FORESTCONTROLLER_H
// #define WIFI_SSID "blabla"
#define WIFI_SSID "iot-net"
#define WIFI_PASS "interactive"

#define MQTT_USERNAME "a0e78aaf"
#define MQTT_PASSWORD "2626bb47aaf15e04"

#define SHORT_CYCLE 150000

#define LEDPIN D2
#define MOISTURE_PIN A0

class ForestController
{
  public:
    ForestController(int id, int amtLeds);
    ~ForestController();
    void setHue(double hue);
    void startLED();
    void loop();
    void setLED(int amount, int brightness, double hue);

  protected:
    int getAmountOfLeds();
    void onMessage(String &topic, String &payload);

    const int totalStrips = 10;
    int stripOffset;
    int ownStrips;

    unsigned long timer;
    unsigned long wifiTimer;
    unsigned long brightnessTimer;

    int id;
    const int minBrightness = 100;
    const int maxBrightness = 255;
    double activatedBrightness = minBrightness;
    int targetBrightness = maxBrightness;
    const unsigned int brightnessInterval = 110;

    double currentHue = 120.0;
    double nextHue = 280.0;
    double targetHue = nextHue;
    double hueIncrease;
    double activatedHue = currentHue;
    int shortCycle = SHORT_CYCLE;
    int timerInterval = shortCycle / totalStrips;

    CRGB strip[6];
    int amountLeds;

    double calculateIncrease(double a, double b, double updateInterval, double time, bool round)
    {
        return updateInterval * ((round ? 2.0 : 1.0) * abs(a - b)) / time;
    }

    double calculateHsvIncrease(double a, double b, double updateInterval, double totalTime)
    {
        double diff = b - a;
        return (diff + ((abs(diff) > 180) ? ((diff < 0) ? 360 : -360) : 0)) * updateInterval / totalTime;
    }
    double brightnessIncrease()
    {
        return 1.1 * calculateIncrease(maxBrightness, minBrightness, brightnessInterval, timerInterval, true);
    }
};

#endif