#include <Adafruit_NeoPixel.h>

int sensorPin = A2;
int sensorValue = 0;

// Initialize the 2 NeoPixel Flora LEDs
Adafruit_NeoPixel colorind = Adafruit_NeoPixel(2, 7, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(9600);
    colorind.begin();
}
void loop() {
    // Read the plant moisture levels from the sensor
    sensorValue = analogRead(sensorPin);
    delay(250);

    // Attach this moisture level to a color between red (bad) and green (good)
    int greenval = (51 * sensorValue)/140 - (255/7);
    int redval = (-51 * sensorValue)/140 + (2040/7);

    // Edge cases for extreme moisture levels
    if (redval > 255) {
      redval = 255;
    }
    if (redval < 0) {
      redval = 0;
    }
    if (greenval > 255) {
      greenval = 255;
    }
    if (greenval < 0) {
      greenval = 0;
    }
    
    // Display data
    colorind.setPixelColor(0, redval, greenval, 0);
    colorind.setPixelColor(1, redval, greenval, 0);

    colorind.show();
}
