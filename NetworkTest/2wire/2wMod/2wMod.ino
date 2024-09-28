#include <USIWire.h>

char CMD;
uint8_t add = 7;

void setup() {
  Wire.begin(add);
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void loop() {
  delay(10);
}

void receiveEvent(int howMany) {
  if(Wire.available() > 0) {
    CMD = Wire.read();
  }
}

void requestEvent() {
  switch(CMD) {
    case 0x01: turnON(); break;
    case 0x02: turnOFF(); break;
  }
}

void turnOFF() {
  digitalWrite(3, LOW);
}

void turnON() {
  digitalWrite(3, HIGH);
}