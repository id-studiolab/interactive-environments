#include <WiFiClientSecure.h>
#include <MQTT.h>

#include "SSD1306Wire.h" 
//initialize the OLED display
SSD1306Wire  display(0x3c, 5, 4);

//wifi settings
const char ssid[] = "used";
const char pass[] = "pass";

//mqtt settings
const char mqtt_clientID[] = "Arduino";
const char mqtt_username[] = "a0e78aaf";
const char mqtt_password[] = "2626bb47aaf15e04";

WiFiClientSecure net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect(mqtt_clientID, mqtt_username, mqtt_password)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  drawMessage(topic,payload);
}


void drawMessage( String topic, String payload) {
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.clear();

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    
    display.drawString(0, 0, "Message received!");
    
    display.drawString(0, 15, "Topic:");
    display.drawString(0, 25, topic);
    
    display.drawString(0, 40, "Message:");
    display.drawString(0, 50, payload);
    
    display.display();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  //
  // MQTT brokers usually use port 8883 for secure connections.
  client.begin("broker.shiftr.io", 8883, net);
  client.onMessage(messageReceived);

  connect();

  //init the display
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/hello", "world");
  }
}
