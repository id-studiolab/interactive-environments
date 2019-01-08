#include "ForestController.h"

ForestController::ForestController(int identifier, int lPerStrip)
{
    id = identifier;
    amountLeds = lPerStrip;
    FastLED.addLeds<NEOPIXEL, LEDPIN>(strip, lPerStrip);
    setLED(lPerStrip, minBrightness, 0);
    FastLED.show();

    pinMode(MOISTURE_PIN, INPUT);

    Serial.println("init completed");
}

void ForestController::setLED(int amount, int brightness, double hue)
{
    int mappedHue = map(hue, 0, 360, 0, 255);
    for (int i = 0; i < amountLeds; i++)
    {
        if (i < amount)
        {
            strip[i].setHSV(mappedHue, 255, brightness);
        }
        else
        {
            strip[i].setRGB(0, 0, 0);
        }
    }
}

void ForestController::setCycleTime(int newCycle)
{
    cycle = newCycle;
}

int ForestController::getAmountOfLeds()
{
    int value = analogRead(MOISTURE_PIN);
    Serial.println(value);
    int amount = map(value, 0, 500, 2, amountLeds);

    return constrain(amount, 2, amountLeds);
}

void ForestController::loop()
{

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

        if (abs(targetHue - activatedHue) > abs(hueIncrease * 2.0))
        {
            activatedHue = fmod(activatedHue + hueIncrease + 360.0, 360.0);
        }

        setLED(getAmountOfLeds(), minBrightness, activatedHue);

        FastLED.show();
    }
}

void ForestController::startLED()
{
    hueIncrease = 2.0 * calculateHsvIncrease(currentHue, nextHue, brightnessInterval, timerInterval);
    activatedHue = currentHue;
    activatedBrightness = minBrightness;
    targetBrightness = maxBrightness;
    targetHue = nextHue;
}

void ForestController::setHue(double hue)
{
    if (abs(hue - nextHue) > 5)
    {
        currentHue = nextHue;
        nextHue = hue;
    }
}