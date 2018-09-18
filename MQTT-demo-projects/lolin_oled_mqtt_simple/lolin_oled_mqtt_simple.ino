#include <WiFi.h>         // make sure hardware support for ESP32 is installed in Arduino!
#include <PubSubClient.h> // search for "PubSubClient" in library manager
#include <U8x8lib.h>      // search for "u8g2" in library manager

#include "_settings.h"    // fill in your access codes here before uploading sketch

// ------- END CONFIGURATION --------

// Initialise OLED with settings for LoLin OLED
// Software I2C on pins 4 and 5
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(4, 5 );

// Allocate static memory for the U8x8log window
uint8_t u8log_buffer[U8LOG_WIDTH * U8LOG_HEIGHT];
U8X8LOG u8x8log;

WiFiClient espClient;
PubSubClient client(espClient);

long last_message_millis = 0;
char mqttOutMessageBuffer[50];
int  mqttOutMessageCounter = 0;

void setup() {
  // Serial.begin(SERIALSPEED);
  u8x8.begin();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);

  // Start U8x8log, connect to U8x8, set the dimension and assign the static memory
  u8x8log.begin(u8x8, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8x8log.setRedrawMode(0);    // 0: Update screen with newline, 1: Update screen for every char
  
  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
