// 
long lastMessageSentTime = 0;
int sendingInterval = 5000;

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
	//send some publish message every sendingInterval amount of time
	if (millis() - lastMessageSentTime > sendingInterval) {
		Serial.print(analogRead(A0));
		Serial.print(" ");
		Serial.println(random(20));
		lastMessageSentTime=millis();
	}

	// receive the data from the serial port
	// data is in the format of "integer space integer newline-character"
	// (if you change this, apply a similar change in the Max patch)
	while (Serial.available()) {
		// parse the valid integers into the RGB led
		int val1 = Serial.parseInt();
		int val2 = Serial.parseInt();

		// look for a newline
		if (Serial.read() == '\n') {
			if (val1 > 0) {
				digitalWrite(13, HIGH);
			} else {
				digitalWrite(13, LOW);
			}
		}
	}
}
