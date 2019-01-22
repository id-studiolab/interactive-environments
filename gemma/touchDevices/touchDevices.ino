//nescessary for all arduino projects
#include <Wire.h>
#include <ADXL345.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include "Adafruit_MPR121.h"
#include <time.h>
#include <TimeLib.h>

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

//id numbers so we can distinguish between the Gemmas
char id[] = "1";

//charging
#define THRESHOLD_CHARGING      3.2

//wifi credentials
  char ssid[] = "iot-net";
  char pass[] = "interactive";
//  char ssid[] = "Bjarke";
//  char pass[] = "testtest";
int timezone = 1;
int dst = 0;

//mqtt credentials
//char mqtt_server[] = "m23.cloudmqtt.com";
//char mqtt_username[] = "ccsycwwb";
//char mqtt_password[] = "iEChr1Rbiax_";
//int mqtt_port = 13154;
char mqtt_server[] = "192.168.1.23";
char mqtt_username[] = "sharing";
char mqtt_password[] = "caring";


WiFiClient net;
MQTTClient client;

//pin connections
#define LED_PIN 12
#define NUM_LEDS 12
#define CHARGE_PIN A0

//initialize led strip and matrix
CRGB leds[NUM_LEDS];
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

//colors
int blue = map(195, 0, 360, 0, 255);
int green = map(123, 0, 360, 0, 255);
int yellow = map(60, 0, 360, 0, 255);
int red = map(0, 0, 360, 0, 255);

ADXL345 adxl; //variable adxl is an instance of the ADXL345 library

//time stuff
float scaleFactor = 1;
unsigned long timerMax = (60/scaleFactor) * 60 * 1000L;
long breakTimer = timerMax;
long taskTimer = 0;
int breakTimerMinutes;
unsigned long breakConstrain = timerMax;
unsigned long lastMillis = 0;
unsigned long currentMillis;
unsigned long lastMqtt = 0;
bool breakTimerOn = true;
bool taskTimerOn = false;

//accelerometer stuff
double xyz[3];
double ax, ay, az, lastAz;
double totalAccel;

//selected timer
bool taskSelected = false;
bool breakSelected = false;
float selectionThreshold = 0.15;
bool taskHighlight = false;
bool breakHighlight = false;

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
int lastTouch = 0;
int touch = 0;

//digits for matrix display
static const uint8_t PROGMEM DIGITS[][8] = {
    {B00000000,
     B11100000,
     B10100000,
     B10100000,
     B10100000,
     B11100000,
     B00000000,
     B00000000},
    {B00000000,
     B11000000,
     B01000000,
     B01000000,
     B01000000,
     B11100000,
     B00000000,
     B00000000},
    {B00000000,
     B11100000,
     B00100000,
     B11100000,
     B10000000,
     B11100000,
     B00000000,
     B00000000},
    {B00000000,
     B11100000,
     B00100000,
     B01100000,
     B00100000,
     B11100000,
     B00000000,
     B00000000},
    {B00000000,
     B10100000,
     B10100000,
     B11100000,
     B00100000,
     B00100000,
     B00000000,
     B00000000},
    {B00000000,
     B11100000,
     B10000000,
     B11100000,
     B00100000,
     B11100000,
     B00000000,
     B00000000},
    {B00000000,
     B11100000,
     B10000000,
     B11100000,
     B10100000,
     B11100000,
     B00000000,
     B00000000},
    {B00000000,
     B11100000,
     B00100000,
     B00100000,
     B00100000,
     B00100000,
     B00000000,
     B00000000},
    {B00000000,
     B11100000,
     B10100000,
     B11100000,
     B10100000,
     B11100000,
     B00000000,
     B00000000},
    {B00000000,
     B11100000,
     B10100000,
     B11100000,
     B00100000,
     B11100000,
     B00000000,
     B00000000}};
const byte CHARGING_IMAGES[][8] = {
{
  B00111100,
  B00100100,
  B01100110,
  B01000010,
  B01000010,
  B01000010,
  B01000010,
  B01111110
},{
  B00111100,
  B00100100,
  B01100110,
  B01000010,
  B01000010,
  B01000010,
  B01111110,
  B01111110
},{
  B00111100,
  B00100100,
  B01100110,
  B01000010,
  B01000010,
  B01111110,
  B01111110,
  B01111110
},{
  B00111100,
  B00100100,
  B01100110,
  B01000010,
  B01111110,
  B01111110,
  B01111110,
  B01111110
},{
  B00111100,
  B00100100,
  B01100110,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110
},{
  B00111100,
  B00100100,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110
},{
  B00111100,
  B00111100,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110
}};
const int CHARGING_LEN = sizeof(CHARGING_IMAGES)/8;

void setup(){
  Wire.begin();
  Serial.begin(9600);

  adxl.powerOn();
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  matrix.begin(0x70); // pass in the address

  WiFi.begin(ssid, pass);
  //client.begin(mqtt_server, mqtt_port, net);
  client.begin(mqtt_server, net);
  client.onMessage(messageReceived);
  connect();

  configTime(timezone * 3600, dst*3600, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }

  if (!cap.begin(0x5B)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  cap.writeRegister(MPR121_ECR, 0x0);
  cap.setThresholds(7, 4);
  cap.writeRegister(MPR121_ECR, 0x8F);
  Serial.println("MPR121 found!");
}

void loop()
{
  static bool foo = 0;
  static int chargecount = 0;
  currentMillis = millis();
  if(isCharging()){
    chargecount = 0;
    foo = 1;
    chargeLoop();
  }
  else{
    if(chargecount<10){
      chargecount++;
    } else {
      if (foo){
      client.publish("/gemma/pickup", id);
      }
      foo = 0;
      timerLoop(); 
    }
  }
  FastLED.show();

  client.loop();
  delay(10);

  if (!client.connected())
  {
    connect();
  }

  //reset once a day
  time_t t = time(nullptr);
  int hourmin = hour(t)*100+minute(t);
  //Serial.println(hourmin);
  if(hourmin == 2359){
    WiFi.forceSleepBegin(); wdt_reset(); ESP.restart(); //while(1)wdt_reset();
  }

  lastMillis = currentMillis;
}

void chargeLoop()
{
    for(int i=0; i<NUM_LEDS; i++){
      leds[i] = CHSV(yellow, 255, 200);
    }

    static unsigned long lastMatrix = 0;
    if (currentMillis-lastMatrix>500){
      lastMatrix = currentMillis;
      static int index = 0;
      matrix.clear();
      matrix.setRotation(2);
      matrix.drawBitmap(0, 0, CHARGING_IMAGES[index], 8, 8, LED_ON);
      matrix.writeDisplay();
      index++;
      if(index>=CHARGING_LEN)
          index = 0;
    }

    // publish a message roughly every ten seconds.
    if (currentMillis - lastMqtt  > 10*1000)
    {
      lastMqtt = currentMillis;
      char str[] = "/gemma/charging/";
      strcat(str, id);
      client.publish(str, "charging");
    }

  //reset timer variables
  breakConstrain=timerMax;
  breakTimer = timerMax;
  taskTimer = 0;
  breakTimerOn = true;
  taskTimerOn = false;

}

void timerLoop()
{
  //update timers
  breakTimer -= (currentMillis-lastMillis);
  taskTimer -= (currentMillis - lastMillis);

  //update data from accelerometer
  updateAcc();

// publish a message roughly every ten seconds.
if (currentMillis - lastMqtt  > 10*1000)
{
  lastMqtt = currentMillis;

  char str[] = "/gemma/availability/";
  strcat(str, id);
  if(ax<=0){
    client.publish(str, "social");
  }
  if(ax>0){
    client.publish(str, "focused");
  }

}


  //check touch surface
  touch = touchCheck();
  //Serial.println(touch);

  if (az< 0-selectionThreshold){
    taskSelected = true;
    breakSelected = false;
    taskHighlight = true;
    breakHighlight = false;
  } else if(az > selectionThreshold){
    taskSelected = false;
    breakSelected = true;
    taskHighlight = false;
    breakHighlight = true;
  }
  else {
    taskSelected = false;
    breakSelected = false;
    taskHighlight = false;
    breakHighlight = false;
  }

  //set timers based on user input
  if (touch!=lastTouch && touch>0){
    if(taskSelected){
      if(ay<0)
        taskTimer = map(touch, 7, 1, 0, timerMax);
      else
        taskTimer = map(touch, 0, 6, 0, timerMax);
      taskTimerOn = true;
    } else if(breakSelected){
      if(ay<0)
        breakTimer = map(touch, 7, 1, 0, timerMax);
      else
        breakTimer = map(touch, 0, 6, 0, timerMax);
      breakTimerOn = true;
    }
  } else {
    //if not touched
  }
  lastTouch = touch;

  if(breakTimer <=0){
    breakTimer = 0;
    if(breakTimerOn){
      breakAlarm(); breakTimerOn = false; taskTimerOn = false;
    }
  }
  if (taskTimer <= 0){
    taskTimer = 0;
    if (taskTimerOn){
      taskAlarm(); taskTimerOn = false;
    }
  }
  if(breakTimer>breakConstrain)
    breakTimer=breakConstrain;
  if(taskTimer>breakTimer)
    taskTimer = breakTimer;

  //set leds according to orientation and timers
  if(ay<=0){
    setLedColor(0, 6, taskTimer, 0, 0, taskHighlight, false);       //set task side to white
    setLedColor(6, 6, breakTimer, blue, 255, breakHighlight, true); //set break side to blue

  } else if(ay>0){
    setLedColor(0, 6, taskTimer, 0, 0, taskHighlight, true);         //set task side to white
    setLedColor(6, 6, breakTimer, green, 255, breakHighlight, false); //set break side to green
  }
  drawNumbers(breakTimer/(60000/scaleFactor));
  delay(100);
}

/*

*************************************************************
                Setup and loop ends here
                Functions are below.
*************************************************************


*/



//connects to mqtt server
void connect()
{
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nwifi connected!");
  char userID[] = "Gemma";
  strcat(userID, id);
  Serial.print("\nconnecting...");
  while (!client.connect(userID, mqtt_username, mqtt_password))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/gemma/pickup");
  //client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

//prints a recieved messagee
void messageReceived(String &topic, String &payload)
{
  Serial.println("incoming: " + topic + " - " + payload);
  if(topic=="pickup"){
    pickupNotification();
  }
}


//updates global values with data from accelerometer
void updateAcc()
{
  adxl.getAcceleration(xyz);
  ax = 0 - xyz[2];
  ay = 0 - xyz[0];
  az = 0 - xyz[1];
  totalAccel = sqrt(ax * ax + ay * ay + az * az);
//  Serial.print(ax);
//  Serial.print(", ");
//  Serial.print(ay);
//  Serial.print(", ");
//  Serial.println(az);
}

//sets leds according to passed timer
//you need to pass the leds you want to control and the hue and saturatiion - value is determined by the timer
//highlight will change the scheme to be slightly lighter. used to indicate to the user that they have selected a side
//the 'up' parameter specifies if the leds should be filing up towards higher or lower index
void setLedColor(int firstLed, int num_leds, long timer, int hue, int saturation, bool highlight, bool up)
{
  int valueMin = highlight ? 55 : 0;
  int valueMax = highlight ? 255 : 200;
  for (int i = firstLed; i < num_leds + firstLed; i++){
  int value = map(
    constrain(timer, (timerMax / num_leds) * (i - firstLed), (timerMax / num_leds) * (i + 1 - firstLed) - 1),
    (timerMax / num_leds) * (i - firstLed),
    (timerMax / num_leds) * (i + 1 - firstLed) - 1,
    valueMin,
    valueMax);
  int activeLed = (up) ? i : map(i, firstLed, firstLed + num_leds - 1, firstLed + num_leds - 1, firstLed);
  leds[activeLed] = CHSV(hue, saturation, value);
  }
}


//shows notification that your task timer has run out
//runs until canceled by shaking
void taskAlarm()
{
  int value = 0;
  bool up = true;
  while(totalAccel<2){
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CHSV(0, 0, value);
    }
    FastLED.show();
    if(up)
      value+=5;
    else
      value-=5;
    if (value >= 251)
      up = false;
    if (value<= 4)
      up = true;
    updateAcc();
    delay(25);
  }
}

//shows notification to take break
//runs until canceled by sliding or shaking
void breakAlarm()
{
  char str[] = "/gemma/break/";
  strcat(str, id);
  client.publish(str, "break");
  delay(10);
  int value = 50;
  bool up = true;
  touch = lastTouch;
  while(touch==lastTouch && totalAccel<2)
  {
    int hue = (ay>=0) ? green : blue;
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CHSV(hue, 255, value);
    }
    FastLED.show();
    if (up)
      value += 5;
    else
      value -= 5;
    if (value >= 251)
      up = false;
    if (value <= 50)
      up = true;
    delay(25);
    touch = touchCheck();
    updateAcc();
  }
  breakConstrain = timerMax/6;
}

void pickupNotification(){
  for(int j=0; j<5; j++){
    leds[5] = CHSV(green, 255, 255);
    leds[6] = CHSV(green, 255, 255);
    FastLED.show();
    delay(100);
    leds[5] = CHSV(0, 0, 0);
    leds[6] = CHSV(0, 0, 0);
    FastLED.show();
    delay(100);
  }
}



//draws the passed number (max 2 digits) on the led matrix display
//uses the DIGITS array specified at the top
void drawNumbers(int number)
{
  int first = number/ 10;
  int second = number % 10;

  if(ay>0){
    matrix.setRotation(2);
  }
  else{
    matrix.setRotation(0);
  }
  matrix.clear();
  matrix.drawBitmap(0, 0, DIGITS[first], 8, 8, LED_ON);
  matrix.drawBitmap(5, 0, DIGITS[second], 8, 8, LED_ON);
  matrix.writeDisplay();
}

//checks voltage on USB pin to detect charging
//returns true if charging, false if not
bool isCharging()
{
  int sensorValue = analogRead(CHARGE_PIN);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  //Serial.println(voltage);
  if (voltage > THRESHOLD_CHARGING)
    return true;
  else
    return false;
}


//checks all the touch keys and returns the touched key
int touchCheck(){
  // Get the currently touched pads
  static uint16_t lasttouched = 0;
  static uint16_t currtouched = 0;
  currtouched = cap.touched();
  //Serial.println(currtouched);

  for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      return i+1;
    }
  }

  return 0;

  // reset our state
  lasttouched = currtouched;
}
