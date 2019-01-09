#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

#define PIN            7
#define NUMPIXELS      12
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, 3);   // Create MFRC522 instance

String uids[6];

int hasUID(String toFind) {
  for (int i = 0; i < 6; i++) {
    if (uids[i] == toFind) {
      return i;
    }
  }
  return -1;
}

int nextIndex() {
  for (int i = 0; i < 6; i++) {
    if (uids[i] == "") {
      return i;
    }
  }
  return -1;
}

void fillUIDS() {
  for (int i = 0; i < 6; i++) {
    uids[i] = "";
  }
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println(F("Read personal data on a MIFARE PICC:"));
  pixels.begin();
  fillUIDS();
}

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
  for (int i = 0; i < 6; i++) {
    if (uids[i] != "") {
      pixels.setPixelColor(i, 50, 50, 50);
      pixels.setPixelColor(11 - i, 50, 50, 50);
    } else {
      pixels.setPixelColor(i, 0, 0, 0);
      pixels.setPixelColor(11 - i, 0, 0, 0);
    }
  }
  pixels.show();

  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String readUID = String(mfrc522.uid.uidByte[0]) + String(mfrc522.uid.uidByte[1]) + String(mfrc522.uid.uidByte[2]) + String(mfrc522.uid.uidByte[3]);
  Serial.println(readUID);
  if (hasUID(readUID) == -1) {
    int next = nextIndex();
    Serial.println("Next free: " + String(next));
    if (next != -1) {
      Serial.println("Table isn't full yet");
      uids[next] = readUID;
    } else {
      Serial.println("Table is full");
      flashLEDS();
    }
  } else {
    Serial.println("Removing: " + readUID);
    uids[hasUID(readUID)] = "";
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(2000); //change value if you want to read cards faster
}
