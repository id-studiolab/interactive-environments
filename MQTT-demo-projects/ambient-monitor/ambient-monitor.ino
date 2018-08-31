#include <WiFi.h>
#include <MQTTClient.h>

const char ssid[] = "iot-net";
const char pass[] = "interactive";

WiFiClient net;
MQTTClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, 19, 18);
  WiFi.begin(ssid, pass);
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("ambient-monitor", "connected-object", "c784e41dd3da48d4")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
}


void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }
  
  // put your main code here, to run repeatedly:
  if (Serial1.available() > 0) {
    char type = Serial1.read();
    Serial1.read();
    String payload = Serial1.readStringUntil(';');
    Serial.print("Type: ");
    Serial.print(type);
    Serial.print(", payload: ");
    Serial.println(payload);

    switch(type) {
      case 't':
        client.publish("/environment/temperature", payload);
        break;
      case 'h':
        client.publish("/environment/humidity", payload);
        break;
      case 'l':
        client.publish("/environment/light", payload);
        break;
      case 's':
        client.publish("/environment/sound", payload);
        break;
      case 'p':
        client.publish("/environment/presence", payload);
        break;
      case 'c':
        client.publish("/environment/CO2", payload);
        break;
    }
    
  }
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}
