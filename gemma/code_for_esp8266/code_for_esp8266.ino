//nescessary for c++ project
//#include <Arduino.h>
//#include <main.h>

//nescessary for all arduino projects
#include <Wire.h>
#include <ADXL345.h>
#include <FastLED.h>
#include <WiFi.h>
#include <MQTT.h>
#include <Adafruit_GFX.h>
#include <Seeed_QTouch.h>
#include <Adafruit_LEDBackpack.h>

//id numbers so we can distinguish between the Gemmas 
char id[] = "1";


//wifi credentials
char ssid[] = "iot-net";
char pass[] = "interactive";

WiFiClient net;
MQTTClient client;

//pin connections
#define LED_PIN 5
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
unsigned long timerMax = scaleFactor * 60 * 60 * 1000L;
long breakTimer = timerMax/2;
long taskTimer = timerMax/3;
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

//touch stuff
int touch;
int lastTouch = 0;
#define key1 0
#define key2 1
#define key3 2
#define key4 4
#define key5 5
#define key6 6

//charging
#define THRESHOLD_CHARGING      1.45

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


void setup(){
  Wire.begin();
  Serial.begin(9600);
  
  WiFi.begin(ssid, pass);
  client.begin("192.168.1.23", net);
  client.onMessage(messageReceived);

  connect();
  adxl.powerOn();
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

  matrix.begin(0x70); // pass in the address
}

void loop()
{
  bool foo = 0;
  currentMillis = millis();
  if(isCharging()){
    foo = 1;
    chargeLoop();
  }
  else{
    if (foo){
      client.publish("pickup", "hello");
    }
    foo = 0;
    timerLoop();
  }
  FastLED.show();

  client.loop();
  delay(10);

  if (!client.connected())
  {
    connect();
  }
  
 
  lastMillis = currentMillis;
}

void chargeLoop()
{
//  int charge = battery.chargePercentage();  // get %
//  battery.reset();  // reset for next data request
//  Serial.print(charge); // print out the battery %
//  Serial.println("%");
//  int hue;
//  if(charge<25){
//    hue = red;
//  } else if(charge<75){
//    hue = yellow;
//  } else {
//    hue = green;
//  }
//  for(int i=0; i<NUM_LEDS; i++){
//    leds[i] = CHSV(hue, 255, 200);
//  }
//  drawNumbers(charge);

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
  touch = QtouchCheck();
  Serial.println(touch);

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
    taskHighlight = false;
    breakHighlight = false;
  }
  
  //set timers based on user input
  if (touch!=lastTouch && touch>0){
    if(taskSelected){
      if(ay>0)
        taskTimer = map(touch, 7, 1, 0, timerMax);
      else
        taskTimer = map(touch, 0, 6, 0, timerMax);
      taskTimerOn = true;
    } else if(breakSelected){
      if(ay>0)
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
    Serial.println("up");
  } else if(ay>0){
    setLedColor(0, 6, taskTimer, 0, 0, taskHighlight, true);         //set task side to white
    setLedColor(6, 6, breakTimer, green, 255, breakHighlight, false); //set break side to green
  }
  drawNumbers(breakTimer/(60000*scaleFactor));
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

  Serial.print("\nconnecting...");
  while (!client.connect("Deskmate", "sharing", "caring"))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

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
  ax = xyz[0];
  ay = xyz[1];
  az = xyz[2];
  totalAccel = sqrt(ax * ax + ay * ay + az * az);
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
  char str[] = "/gemmaa/break/";
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
    touch = QtouchCheck();
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
  // print out the value you read:
  //Serial.println(voltage);
  if (voltage > THRESHOLD_CHARGING)
    return true;
  else
    return false;
}

//run in setup function
void QtouchSetup(){
  QTouch.calibrate();

  //the value determines how long any key can be in touch before it recalibrates itself.160ms * 62 = 9.92s.
  QTouch.setMaxDuration(62);

  //set the threshold value for key0 to register a detection.This Value should not be less than 20.
  QTouch.setNTHRForKey(15, key1);
  QTouch.setNTHRForKey(20, key2);
  QTouch.setNTHRForKey(20, key3);
  QTouch.setNTHRForKey(20, key4);
  QTouch.setNTHRForKey(20, key5);
  QTouch.setNTHRForKey(25, key6);
}

//checks all the touch keys and returns the touched key
//if none is touched returns 0
int QtouchCheck(){
  if (QTouch.isTouch(key1)) {
    return 1;
    //Serial.println(a);//"key 1");
    //while(QTouch.isTouch(key1));
  } else if (QTouch.isTouch(key2)) {
    return 2;
    //Serial.println(a);//"key 2");
    //while(QTouch.isTouch(key2));
  } else if (QTouch.isTouch(key3)) {
    return 3;
    //Serial.println(a);//"key 3");
    //while(QTouch.isTouch(key3));
  } else if (QTouch.isTouch(key4)) {
    return 4;
    //Serial.println(a);//"key 4");
    //while(QTouch.isTouch(key4));
  } else if (QTouch.isTouch(key5)) {
    return 5;
    //Serial.println(a);//"key 5");
    //while(QTouch.isTouch(key5));
  } else if (QTouch.isTouch(key6)) {
    return 6;
    //Serial.println(a);//"key 6");
    //while(QTouch.isTouch(key6));
  } else {
    return 0;
  }
}
