// ---------------
// -- Libraries --
// ---------------

/*********************************************************
  FastLED Library
**********************************************************/

#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define CLOCK_PIN     D3
#define INNER_PIN     D5
#define MIDDLE_PIN    D6
#define OUTER_PIN     D7
#define LED_TYPE      WS2813
#define COLOR_ORDER   GRB
#define NUMPIXELS  42
#define NUMPIXELSSMALL     13
CRGB innerpixels[NUMPIXELS];
CRGB outerpixels[NUMPIXELS];
CRGB innerclock[NUMPIXELSSMALL];
#define BRIGHTNESS    70

/*********************************************************
  CmdMessenger Library
**********************************************************/

#include <CmdMessenger.h>
const int BAUD_RATE = 9600;
CmdMessenger c = CmdMessenger(Serial, ',', ';', '/');

/*********************************************************
  Wifi + MQTT
**********************************************************/

//#include <ESP8266WiFi.h>
//#include <MQTT.h>
//
//const char ssid[] = "iot-net";
//const char pass[] = "interactive";
//
//WiFiClient net;
//MQTTClient client;

// ---------------
// -- Variables --
// ---------------

int huetaken = 0; //ams rood
int sattaken = 255;
int valtaken = 255;
int valdim = 150;
int satdim = 200;
int huefree = 0;
int saturationfree = 0;
int brightnessfree = 255;

const int BOOKING_SIZE = 24;
long startbookings[BOOKING_SIZE];
int startbookingsIndex = 0;
long endbookings[BOOKING_SIZE];
int endbookingsIndex = 0;

int NUMevents = 0;
int clearval;
boolean lateled;

struct led {
  int red = 0;
  int green = 0;
  int blue = 0;
}
led [3];

struct meeting {
  int starttime;
  int endtime;
}
meeting [3];

enum {
  error,
  send_starttime,
  send_endtime,
  clear_array,
  s,
};

int huetest = 50;
int hueon = 0;
int saton = 0;
int valon = 255;
int hueoff = 0;
int satoff = 0;
int valoff = 0;
int current[1];
long currenttime;
int aDelay = 5000;
int currenttimemod;
int currentstrip;
int timeslot;
int duration;
int received;
int oldreceived;
int lastcurrent;
int countdown;
long timer;

// ------------------
// -- Setup & Loop --
// ------------------

void setup() {
  //  WiFi.begin(ssid, pass);
  //  client.begin("192.168.1.23", net);
  //  client.onMessage(messageReceived);
  //  connect();
  Serial.begin(BAUD_RATE);
  attach_callbacks();
  delay(1000);
  FastLED.addLeds<LED_TYPE, INNER_PIN, COLOR_ORDER>(innerpixels, NUMPIXELS);
  FastLED.addLeds<LED_TYPE, MIDDLE_PIN, COLOR_ORDER>(innerpixels, NUMPIXELS);
  FastLED.addLeds<LED_TYPE, OUTER_PIN, COLOR_ORDER>(outerpixels, NUMPIXELS);
  FastLED.addLeds<LED_TYPE, CLOCK_PIN, COLOR_ORDER>(innerclock, NUMPIXELSSMALL);
  LEDS.setBrightness(BRIGHTNESS);
  currenttime = millis();
  timer = millis();
}

void loop() {
  c.feedinSerialData();
  //  client.loop();
  //  delay(10);
  //  if (!client.connected()) {
  //    connect();
  //  }

  setmeetingTimes();
  setdisplay();
  show_time();
  dim_strips();
  FastLED.show();

  delay(1000);

  //  for (int i = 0; i < NUMevents; i++) {
  //    if ((current[0] * 900)  > endbookings[NUMevents - 1]) {
  //      client.publish("/meet/daily/", "no more events");
  //    }
  //    else if ((current[0] * 900) >= startbookings[i] && (current[0] * 900) < endbookings[i]) {
  //      long timeslot = (endbookings[i] - startbookings[i]);
  //
  //      String timewindow = "";
  //      timewindow += timeslot;
  //      timewindow += ".";
  //      timewindow += 0;
  //
  //      client.publish("/meet/daily/", timewindow);
  //      break;
  //    }
  //    else if ((current[0] * 900) > endbookings[i] && (current[0] * 900) < startbookings[i + 1]) {
  //      long timeslot = (endbookings[i + 1] - startbookings[i + 1]);
  //      long timetillnext = (startbookings[i + 1] - (current[0] * 900));
  //
  //      String timewindow = "";
  //      timewindow += timeslot;
  //      timewindow += ".";
  //      timewindow += timetillnext;
  //
  //      client.publish("/meet/daily/", timewindow);
  //      break;
  //    }
  //    else if ((current[0] * 900) > startbookings[i] && (current[0] * 900) > endbookings[i]) {
  //      //
  //    }
  //    else if ((current[0] * 900) < startbookings[i] && i == 0) {
  //      long timeslot = (endbookings[i] - startbookings[i]);
  //      long timetillnext = (startbookings[i] - (current[0] * 900));
  //
  //      String timewindow = "";
  //      timewindow += timeslot;
  //      timewindow += ".";
  //      timewindow += timetillnext;
  //
  //      client.publish("/meet/daily/", timewindow);
  //      break;
  //    }
  //    else {
  //      client.publish("/meet/daily/", "error");
  //    }
  //  }
}

// ------------------------
// -- Callback Functions --
// ------------------------

void on_unknown_command(void) {
  c.sendCmd(error, "Command without callback.");
}

void on_send_starttime(void) {
  long start1 = c.readBinArg<long>();
  startbookings[startbookingsIndex] = start1;
  startbookingsIndex++;
}

void on_send_endtime(void) {
  long endtime = c.readBinArg<long>();
  endbookings[endbookingsIndex] = endtime;
  endbookingsIndex++;
  NUMevents = endbookingsIndex;
}

void on_clear_array(void) {
  int clearval = c.readBinArg<int>();
  if (clearval = 1) {
    for ( int i = 0; i < BOOKING_SIZE;  ++i ) {
      startbookings[i] = 0;
      endbookings[i] = 0;
    }
    delay(3000);
    startbookingsIndex = 0;
    endbookingsIndex = 0;
    for (int i = 0; i < NUMPIXELS; i++) {
      innerpixels[i] = CHSV(huefree, saturationfree, brightnessfree);
      outerpixels[i] = CHSV(huefree, saturationfree, brightnessfree);
    }
  }
}

void on_send_currenttime(void) {
  int currenttime = c.readBinArg<int>();
  c.sendBinCmd(s, currenttime);
  current[0] = currenttime;
}

void attach_callbacks(void) {
  c.attach(on_unknown_command);
  c.attach(send_starttime, on_send_starttime);
  c.attach(send_endtime, on_send_endtime);
  c.attach(clear_array, on_clear_array);
  c.attach(s, on_send_currenttime);
}

// -------------
// -- Actions --
// -------------

void setmeetingTimes() {
  for (int i = 0; i < NUMevents; i++) {
    meeting[i].starttime = startbookings[i];
    meeting[i].endtime = endbookings[i];
  }
}

void setdisplay() {
  for (int i = 0; i < NUMevents; i++) {
    for (int j = (startbookings[i] / 900) - 32; j < (endbookings[i] / 900) - 32; j++) {
      if ((j >= 0) && (j <= 40)) {
        innerpixels[j + 1] = CHSV(huetaken, sattaken, valtaken);
        outerpixels[(NUMPIXELS - 1) - (j + 1)] = CHSV(huetaken, sattaken, valtaken);
      }
      if (j < 0) {
        innerpixels[0] = CHSV(huetaken, sattaken, valtaken);
        outerpixels[41] = CHSV(huetaken, sattaken, valtaken);
      }
      if (j > 40) {
        innerpixels[41] = CHSV(huetaken, sattaken, valtaken);
        outerpixels[0] = CHSV(huetaken, sattaken, valtaken);
      }
    }
  }
}

void show_time() {
  if (current[0] > 32 && current[0] < 72) {
    currenttimemod = (current[0] - 32) / 4;  //current[0]
    for (int i = 0; i < 12; i++) {
      if (i < currenttimemod) {
        innerclock[i] = CHSV(hueoff, satoff, valoff);
      }
      else {
        innerclock[i] = CHSV(hueon, saton, valon);
      }
    }
  }
  if (current[0] > 0 && current[0] < 32) {
    innerclock[12] = CHSV(hueon, saton, valon);
  }
  if (current[0] > 72 && current[0] < 96) {
    innerclock[11] = CHSV(hueon, saton, valon);
  }
}

void dim_strips() {
  if (current[0] > 31 && current[0] < 73) {
    for (int i = 0; i < (current[0] - 31); i++) {  //(current[0] - 31)
      innerpixels[i] = CHSV(huetaken, satdim, valdim);
      outerpixels[41 - i] = CHSV(huetaken, satdim, valdim);
    }
  }
  else if (current[0] > 73) {
    for (int i = 0; i < 42; i++) {
      innerpixels[i] = CHSV(huetaken, satdim, valdim);
      outerpixels[41 - i] = CHSV(huetaken, satdim, valdim);
    }
  }
}

void adhoc() {
  if (received != oldreceived) {
    if (received == 2) {
      duration = received + 1;
    }
    else if (received == 1) {
      duration = received - 1;
    }
    else if (received == 0) {
      duration = received;
    }
    oldreceived = received;
  }

  if (current[0] > lastcurrent) {
    countdown = duration - 1;
    lastcurrent = current[0];
  }

  for (int i = current[0]; i < (current[0] + countdown); i++) {
    if (countdown > 0 && i >= 32 && i <= 72) {
      innerpixels[i] = CHSV(huetaken, sattaken, valtaken);
      outerpixels[NUMPIXELS - i] = CHSV(huetaken, sattaken, valtaken);
    }
  }
}

//void connect() {
//    while (WiFi.status() != WL_CONNECTED) {
//      delay(1000);
//    }
//    while (!client.connect("DailyMeet", "sharing", "caring")) {
//      delay(1000);
//    }
//    client.subscribe("/meet/now/");
//}

void messageReceived(String & topic, String & payload) {
  String meetduration = getValue(payload, '.', 0);
  String plusminus = getValue(payload, '.', 1);
  duration = meetduration.toInt();
  received = plusminus.toInt();
  Serial.println(duration);
  Serial.println(received);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


