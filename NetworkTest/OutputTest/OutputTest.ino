#include <SoftwareSerial.h>

//ATTINY85 settings
#define ITX PB3;
#define IRX PB4;
#define OTX PB2;
#define ORX PB1;
#define LED PB0;

SoftwareSerial input = SoftwareSerial(4, 3);
SoftwareSerial output = SoftwareSerial(1, 2);

void setup() {
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  input.begin(9600);
  output.begin(9600);
  char h = '0';
  
  delay(5000);
  int sent = output.write(h);

    for(int i=0;i<sent;i++) {
    delay(1000);
    digitalWrite(0, LOW);
    delay(1000);
    digitalWrite(0, HIGH);
  }
}

void loop() {
  input.listen();
  if(input.available()) {
    char c = input.read();
    if(c == '0') {
      digitalWrite(0, LOW);
    }
  }
}