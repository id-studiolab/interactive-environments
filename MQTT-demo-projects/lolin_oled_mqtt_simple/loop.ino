void loop() {
  
  // reconnect if necessary
  if (!client.connected()) { connect(); }
  // process MQTT messages
  client.loop();

  // dummy MQTT posting
  long now = millis();
  if (now - last_message_millis > 20000) {
    last_message_millis = now;
    ++mqttOutMessageCounter;
    snprintf (mqttOutMessageBuffer, 75, "helloworld #%ld", mqttOutMessageCounter);

    u8x8log.print  ("o "); // for OUT
    u8x8log.println(mqttOutMessageBuffer);
    client.publish (mqtt_publishTo, mqttOutMessageBuffer);
  }
}
