#include <DHT.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif
#define sensor s_serial
#define NUMPIXELS      16
#define HUMIDPIN 9
#define DHTTYPE DHT22

Adafruit_NeoPixel tempring = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel humidring = Adafruit_NeoPixel(NUMPIXELS, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel co2ring = Adafruit_NeoPixel(NUMPIXELS, 4, NEO_GRB + NEO_KHZ800);
SoftwareSerial s_serial(2, 3);
DHT humiditySensor(HUMIDPIN, DHTTYPE);

const unsigned char cmd_get_sensor[] = {
  0xff, 0x01, 0x86, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x79
};

int CO2PPM;
int sendCounter = 0;

const long interval = 5000;
int prevTime = 0;

void setup() {
  sensor.begin(9600);
  Serial.begin(9600);

  humiditySensor.begin();
  tempring.begin();
  humidring.begin();
  co2ring.begin();
}

void loop() {
  float humidity = humiditySensor.readHumidity();
  float temperature = humiditySensor.readTemperature();

  dataRecieve();

  int tempamount = map(temperature, 15, 25, 1, 15);
  int humidamount = map(humidity, 0, 100, 1, 15);
  
  int co2amount = 1;
  if (CO2PPM <= 1200) {
    co2amount = map(CO2PPM, 200, 1200, 1, 15);
  } else {
    co2amount = 15;
  }

  for (int i = 0; i < NUMPIXELS; i++) {
    tempring.setPixelColor(i, 0, 0, 0);
    humidring.setPixelColor(i, 0, 0, 0);
    co2ring.setPixelColor(i, 0, 0, 0);
  }

  for (int i = 0; i < tempamount; i++) {
    tempring.setPixelColor(15-i, 15, 15, 15);
  }
  for (int i = 0; i < humidamount; i++) {
    humidring.setPixelColor(15-i, 15, 15, 15);
  }
  for (int i = 0; i < co2amount; i++) {
    co2ring.setPixelColor(15-i, 15, 15, 15);
  }
  tempring.show();
  humidring.show();
  co2ring.show();

  Serial.println(String(temperature) + "T");
  Serial.println(String(humidity) + "H");
  Serial.println(String(CO2PPM) + "C");
  
  delay(interval);
}

bool dataRecieve(void) {
  byte data[9];
  int i = 0;

  for (i = 0; i < sizeof(cmd_get_sensor); i++) {
    sensor.write(cmd_get_sensor[i]);
  }
  delay(10);

  if (sensor.available()) {
    while (sensor.available()) {
      for (int i = 0; i < 9; i++) {
        data[i] = sensor.read();
      }
    }
  }

  if ((i != 9) || (1 + (0xFF ^ (byte)(data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7]))) != data[8]) {
    return false;
  }

  CO2PPM = (int)data[2] * 256 + (int)data[3];

  return true;
}
