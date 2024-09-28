#include <Wire.h>

const uint8_t defaultAddress = 1;
uint16_t timer = 0;

uint8_t code = 0;
uint8_t address = 0;
uint8_t attempts = 0;
size_t numR = 16;
char buffer[16];

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  if (timer >= 1000) {
    checkforDefaults();
  } else {
    timer++;
  }

  while(Serial.available()) {
    String p = Serial.readString();
    if(p == "on\n") {
      code = 0x01;
      address = 7;
      requestBegin();
    } else if(p == "off\n") {
      code = 0x02;
      address = 7;
      requestBegin();
    }
  }
}

void checkForDefaults() {
  requestFrom(defaultAddress, 6);
};

void requestBegin() {
  Wire.beginTransmission(address);
  Wire.write(code);
  Wire.endTransmission();
  waitForAck();
}

int waitForAck() {
  uint8_t bufferIndex = 0;
  while(code) {
    while(Wire.available()) {
      buffer[bufferIndex] = Wire.read();
      if(bufferIndex >= numR-1) {
        Serial.print(buffer);
        code = 0;
        attempts = 0;
        return 0;
      }
      bufferIndex++;
    }
    if(attempts > 20) {
      Serial.print("Timed Out.\n");
      code = 0;
      attempts = 0;
      return 1;
    }
    attempts++;
  }
  Serial.println(buffer);
}