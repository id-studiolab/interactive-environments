#include <Adafruit_NeoPixel.h>

int sensorPin = A2;
int sensorValue = 0;

Adafruit_NeoPixel colorind = Adafruit_NeoPixel(2, 7, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(9600);
    colorind.begin();
}
void loop() {
    // read the value from the sensor:
    sensorValue = analogRead(sensorPin);
    Serial.print("Moisture = " );
    Serial.println(sensorValue);
    delay(250);

    int greenval = (51 * sensorValue)/140 - (255/7);
    int redval = (-51 * sensorValue)/140 + (2040/7);

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
    
    colorind.setPixelColor(0, redval, greenval, 0);
    colorind.setPixelColor(1, redval, greenval, 0);

    colorind.show();
}
