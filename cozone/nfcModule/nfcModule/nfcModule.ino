#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

// Initialize NFC LED strip (2 rows of 6 pixels)
#define PIN            7
#define NUMPIXELS      12
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Define 2 pins for the MFRC522 NFC board
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, 3);   // Create MFRC522 instance

// Create data structure for the "checked-in" keycard UIDs
String uids[6];

int prevTime;

// Function that returns the index of the to be found UID
int hasUID(String toFind) {
  for (int i = 0; i < 6; i++) {
    if (uids[i] == toFind) {
      return i;
    }
  }
  return -1;
}

// Function that returns the next available index for the UID data structure
int nextIndex() {
  for (int i = 0; i < 6; i++) {
    if (uids[i] == "") {
      return i;
    }
  }
  return -1;
}

// Function that is used to initialize the UID data structure
void fillUIDS() {
  for (int i = 0; i < 6; i++) {
    uids[i] = "";
  }
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pixels.begin();
  fillUIDS();
  prevTime = millis();
}

// Function that fleshes all NFC LEDs red once if this CoZone is full
void flashLEDS() {
  for (int i = 0; i < 6; i++) {
    pixels.setPixelColor(i, 50, 0, 0);
  }
  pixels.show();
  delay(300);
  for (int i = 0; i < 6; i++) {
    pixels.setPixelColor(i, 0, 0, 0);
  }
  pixels.show();
  delay(300);
  for (int i = 0; i < 6; i++) {
    pixels.setPixelColor(i, 50, 0, 0);
  }
  pixels.show();
  delay(300);
  for (int i = 0; i < 6; i++) {
    pixels.setPixelColor(i, 0, 0, 0);
  }
  pixels.show();
  delay(300);
  for (int i = 0; i < 6; i++) {
    pixels.setPixelColor(i, 50, 0, 0);
  }
  pixels.show();
  delay(300);
  for (int i = 0; i < 6; i++) {
    pixels.setPixelColor(i, 0, 0, 0);
  }
  pixels.show();
}

void loop() {
  int amount = 0;
  
  // Turn on the lights if someone is checked in
  for (int i = 0; i < 6; i++) {
    if (uids[i] != "") {
      amount++;
      pixels.setPixelColor(i, 50, 50, 50);
      pixels.setPixelColor(11 - i, 50, 50, 50);
    } else {
      pixels.setPixelColor(i, 0, 0, 0);
      pixels.setPixelColor(11 - i, 0, 0, 0);
    }
  }
  pixels.show();

  // Send data to the attached WeMoS board every other 5 seconds
  if (millis() - prevTime > 5000) {
    Serial.println(String(amount));
    prevTime = millis();
  }

  // Edge cases for malfunctioning RFID-Scanner
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Read the UID if an RFID-Tag is presented to the module
  String readUID = String(mfrc522.uid.uidByte[0]) + String(mfrc522.uid.uidByte[1]) + String(mfrc522.uid.uidByte[2]) + String(mfrc522.uid.uidByte[3]);
  // If the card isn't recognize, initialize it, otherwise log the person out.
  if (hasUID(readUID) == -1) {
    int next = nextIndex();
    // If there's a spot available, save the tag UID, otherwise flash
    if (next != -1) {
      uids[next] = readUID;
    } else {
      flashLEDS();
    }
  } else {
    uids[hasUID(readUID)] = "";
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(500); //change value if you want to read cards faster
}
