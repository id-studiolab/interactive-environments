#include <Wire.h>
#include <Digital_Light_ISL29035.h>
#include <Digital_Light_TSL2561.h>
#include <Reactduino.h>
#include <SoftwareSerial.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22
#define SOUND_PIN A0
#define PIR_PIN 3

SoftwareSerial serialComm(7, 8);

SoftwareSerial sensor(4, 5);
const unsigned char cmd_get_sensor[] =
{
    0xff, 0x01, 0x86, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x79
};
unsigned char dataRevice[9];
int temperature;
int CO2PPM;

DHT dht(DHTPIN, DHTTYPE);

void sendMessage(String type, String payload) {
  String message = type + ":" + payload + ";";
  serialComm.print(message);
}

bool dataReceive(void)
{
    byte data[9];
    int i = 0;

    //transmit command data
    for(i=0; i<sizeof(cmd_get_sensor); i++)
    {
        sensor.write(cmd_get_sensor[i]);
    }
    delay(10);
    //begin reveiceing data
    if(sensor.available())
    {
        while(sensor.available())
        {
            for(int i=0;i<9; i++)
            {
                data[i] = sensor.read();
            }
        }
    }

    for(int j=0; j<9; j++)
    {
        Serial.print(data[j]);
        Serial.print(" ");
    }
    Serial.println("");

    if((i != 9) || (1 + (0xFF ^ (byte)(data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7]))) != data[8])
    {
        return false;
    }

    CO2PPM = (int)data[2] * 256 + (int)data[3];
    temperature = (int)data[4] - 40;

    return true;
}

void sampleCO2() {
  if (dataReceive()) {
    sendMessage("c", String(CO2PPM));
  }
}

void sampleSound() {
  int s = analogRead(SOUND_PIN);
  sendMessage("s", String(s));
}

void samplePresence() {
  int p = digitalRead(PIR_PIN);
  sendMessage("p", String(p));
}

void sampleHumidity() {
  float h = dht.readHumidity();
  if (!isnan(h)) {
    sendMessage("h", String(h));
  } 
}

void sampleTemperature() {
  float t = dht.readTemperature();
  if (!isnan(t)) {
    sendMessage("t", String(t));
  }
}

void sampleLight() {
  int l = TSL2561.readVisibleLux();
  sendMessage("l", String(l));
}

void loop_main() {
  
}

void app_main() {
  Serial.begin(9600);
  serialComm.begin(9600);
  pinMode(SOUND_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  dht.begin();
  Wire.begin();
  TSL2561.init();
  sensor.begin(9600);
  app.repeat(100, sampleSound);
  app.repeat(200, samplePresence);
  app.repeat(10000, sampleTemperature);
  app.repeat(10000, sampleHumidity);
  app.repeat(500, sampleLight);
  app.repeat(5000, sampleCO2);
  app.onTick(loop_main);
}

Reactduino app(app_main);
