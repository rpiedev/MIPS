#include <SoftwareSerial.h>

//Arduino Nano Every settings
const int ITX = 9;
const int IRX = 10;
const int OTX = 11;
const int ORX = 12;

SoftwareSerial input = SoftwareSerial(IRX, ITX);
SoftwareSerial output = SoftwareSerial(ORX, OTX);

void setup() {
  input.begin(9600);
  //output.begin(9600);
  Serial.begin(9600);
}

void loop() {
  input.listen();
  if(input.available()) {
    Serial.println(input.read());
  }
  
  delay(100);
}