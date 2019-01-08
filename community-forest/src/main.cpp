#include <Arduino.h>
#include "ForestController.h"

ForestController *controller;

void setup()
{
    Serial.begin(115200);
    controller = new ForestController(0, 6);
}

void loop()
{
    controller->loop();
    delay(10);
}