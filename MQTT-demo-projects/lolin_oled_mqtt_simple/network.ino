void setup_wifi() { // connects to wifi
  
  delay(10);
  u8x8log.println ("WiFi:");
  u8x8log.println (ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) { delay(500);  }
  u8x8log.println(WiFi.localIP());
}

void connect() { // (re)connects to wifi and mqtt including one subscription
  while (WiFi.status() != WL_CONNECTED) { delay(1000); }

  u8x8log.print ("MQTT ... ");
  while (!client.connect(mqtt_client_id, mqtt_username, mqtt_password)) { delay(1000); }
  u8x8log.println("OK");

  client.subscribe(mqtt_subscribeTo);

  u8x8log.print("pub: ");
  u8x8log.println(mqtt_publishTo);

  u8x8log.print("sub: ");
  u8x8log.println(mqtt_subscribeTo);
}
