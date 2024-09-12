#include <SoftwareSerial.h>

//ATTINY85 settings
#define ITX PB3;
#define IRX PB4;
#define OTX PB2;
#define ORX PB1;
#define LED PB0;

int main() {
  SoftwareSerial input = SoftwareSerial(4, 3);
  SoftwareSerial output = SoftwareSerial(1, 2);
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  input.begin(9600);
  output.begin(9600);
  
  while(1) {
    input.listen();
    while(input.available() > 0) {
      uint8_t inByte = input.read();
      if(inByte == 'H') { //01001000
        digitalWrite(0, HIGH);
      } else if(inByte == 0) { //01001100
        digitalWrite(0, LOW);
      }
      output.write(inByte);
    }
    
    output.listen();
    while(output.available() > 0) {
      char inByte = input.read();
      input.write(inByte);
    } 
  }
  return 0;
}
