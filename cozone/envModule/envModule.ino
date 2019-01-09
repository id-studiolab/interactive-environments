#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <math.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

WiFiClient net;
WiFiClient client;
HTTPClient http;

char server[] = "api.openweathermap.org";
String apiKey = "128e47add9cff5fc178fa1011767c49b";
//the city you want the weather for
String location = "Amsterdam,NL";
int status = WL_IDLE_STATUS;

StaticJsonBuffer<8192> jsonBuffer;
int countdown = 0;

const char ssid[] = "Science-Centre-EVENT";
const char pass[] = "ScienceCentre";

#define PIN            2
#define NUMPIXELS      8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel outsidetemp = Adafruit_NeoPixel(15, D2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel humidity = Adafruit_NeoPixel(15, D5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel windspeed = Adafruit_NeoPixel(15, D1, NEO_GRB + NEO_KHZ800);

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  getWeather();
}

void getWeather() {
  Serial.println("\nStarting connection to server...");
  if (client.connect(server, 80)) {
    Serial.println("connected to server\n");
    client.print("GET /data/2.5/forecast?");
    client.print("q=" + location);
    client.print("&appid=" + apiKey);
    client.print("&cnt=3");
    client.println("&units=metric");
    client.println("Host: api.openweathermap.org");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("unable to connect");
  }
  delay(1000);
  String line = "";
  while (client.connected()) {
    line = client.readStringUntil('\n');
  }
  client.flush();
  client.stop();
  JsonObject& root = jsonBuffer.parseObject(line);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  int weather_id = root["list"][0]["weather"][0]["id"];
  float temp = root["list"][0]["main"]["temp"];
  float wind = root["list"][0]["wind"]["speed"];
  int humidityv = root["list"][0]["main"]["humidity"];

  Serial.println("Wind speed: " + String(wind) + " m/s");
  Serial.println("Temp out: " + String(temp) + " degrees");
  Serial.println("Humidity out: " + String(humidityv) + "%");

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, 0, 0, 0);
  }

  if (weather_id >= 200 and weather_id < 300) {
    // Thunderstorm
    pixels.setPixelColor(4, 255, 255, 255);
  } else if (weather_id >= 300 and weather_id < 600) {
    // Drizzle / Rain
    pixels.setPixelColor(2, 255, 255, 255);
  } else if (weather_id >= 600 and weather_id < 700) {
    // Snow
    pixels.setPixelColor(5, 255, 255, 255);
  } else if (weather_id == 701 or weather_id == 721 or weather_id == 741) {
    // Mist
    pixels.setPixelColor(3, 255, 255, 255);
  } else if (weather_id == 800) {
    // Clear Sky
    pixels.setPixelColor(0, 255, 255, 255);
  } else if (weather_id >= 801 and weather_id <= 804) {
    // Heavy Clouds
    pixels.setPixelColor(1, 255, 255, 255);
  }

  for (int i = 0; i < 15; i++) {
    outsidetemp.setPixelColor(15-i, 0, 0, 0);
    windspeed.setPixelColor(15-i, 0, 0, 0);
    humidity.setPixelColor(15-i, 0, 0, 0);
  }
  int temp_a = map(temp, 0, 30, 1, 15);
  int wind_a = map(wind, 0, 25, 1, 15);
  int humid_a = map(humidityv, 0, 100, 1, 15);
  for (int i = 0; i < temp_a; i++) {
    outsidetemp.setPixelColor(15-i, 15, 15, 15);
  }
  for (int i = 0; i < wind_a; i++) {
    windspeed.setPixelColor(15-i, 15, 15, 15);
  }
  for (int i = 0; i < humid_a; i++) {
    humidity.setPixelColor(15-i, 15, 15, 15);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(ssid, pass);

  connect();
  pixels.begin();
  outsidetemp.begin();
  humidity.begin();
  windspeed.begin();
  delay(1000);
}

void loop() {
  delay(10);

  if (millis() - countdown > 300000) {
    getWeather();
    countdown = millis();
  }
  pixels.show();
  outsidetemp.show();
  humidity.show();
  windspeed.show();
}
