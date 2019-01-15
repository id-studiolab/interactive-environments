#include "ForestController.h"

ForestController::ForestController()
{
    id = CONTROLLER_ID;
    this->amountLeds = AMOUNT_LEDS;
    FastLED.addLeds<NEOPIXEL, LEDPIN>(strip, amountLeds);
    setLED(AMOUNT_LEDS, minBrightness, 0);
    FastLED.show();
    // strip[6].setRGB(0, 0, 0);

    pinMode(MOISTURE_PIN, INPUT);

    Serial.println("init completed");
    Serial.println(amountLeds);
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

int ForestController::getAmountOfLeds()
{
    if (!moistureOn)
    {
        return amountLeds;
    }
    int value = getMoistureSensorValue();
    int amount = map(value, 0, 500, 2, amountLeds);

    if (amount < 2)
        amount = 2;
    if (amount > amountLeds)
        amount = amountLeds;

    Serial.println("amount = " + String(amount));
    return amount;
}

int ForestController::getMoistureSensorValue()
{
    return analogRead(MOISTURE_PIN);
}

void ForestController::enableMoisture(bool enable)
{
    moistureOn = enable;
}

void ForestController::loop()
{
    // Serial.println(this->amountLeds);
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

        if (abs(targetHue - currentHue) > abs(hueIncrease * 2.0))
        {
            currentHue = fmod(currentHue + hueIncrease + 360.0, 360.0);
        }
        setLED(getAmountOfLeds(), activatedBrightness, currentHue);

        FastLED.show();
    }
}

void ForestController::startLED(double hue, double timeTaken)
{
    if (abs(hue - targetHue) > 5)
    {
        timerInterval = timeTaken;
        hueIncrease = 2.0 * calculateHsvIncrease(currentHue, hue, brightnessInterval, timerInterval);
        targetBrightness = maxBrightness;
        targetHue = hue;
    }
}