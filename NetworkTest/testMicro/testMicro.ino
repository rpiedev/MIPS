#include <SoftwareSerial.h>

//ATTINY85 settings
const int ITX = 20;
const int IRX = 21;
const int OTX = 22;
const int ORX = 23;

SoftwareSerial input = SoftwareSerial(IRX, ITX);
SoftwareSerial output = SoftwareSerial(ORX, OTX);

void setup() {
  input.begin(9600);
  output.begin(9600);
  Serial.begin(9600);
  char h = '0';

  delay(1000);
  int sent = output.write('N');
}

void loop() {
  
}