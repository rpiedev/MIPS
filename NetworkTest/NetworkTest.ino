
int main() {
  char *buffer = (char *)malloc(sizeof(char)*32);
  
}


/*#include <SoftwareSerial.h>

//ATTINY85 settings
const int ITX = 3;
const int IRX = 4;
const int OTX = 2;
const int ORX = 1;
const int LED = 0;

char packetBuffer[32]; 
uint8_t packetBufferCurrent = 0;
uint8_t packetBufferSize = 0;
bool bufferFull = 0;
char currentChar = '\0';

SoftwareSerial input = SoftwareSerial(IRX, ITX);
SoftwareSerial output = SoftwareSerial(ORX, OTX);

void setup() {
  input.begin(9600);
  output.begin(9600);
  delay(5000);
}

void loop() {
  input.listen();
  if(input.available() > 0) {
    while(bufferFull == 0) {
      currentChar = input.read();
      packetBuffer[packetBufferCurrent] = currentChar;
      packetBufferCurrent++;
      packetBufferSize++;
      if(packetBufferSize >= 32 || currentChar == 4) {
        bufferFull = 1;
        packetBufferCurrent = 0;
      }
    }
  }
  while(bufferFull == 1) { 
    currentChar = packetBuffer[packetBufferCurrent];
    output.write(currentChar);
    packetBufferCurrent++;
    if(packetBufferCurrent == packetBufferSize) { // End of Transmission
      bufferFull = 0;
      packetBufferCurrent = 0;
      packetBufferSize = 0;
    }
  }
  /*
  output.listen();
  delay(5);
  if(output.available()> 0) {
    while(bufferFull == 0) {
      currentChar = output.read();
      packetBuffer[packetBufferCurrent] = currentChar;
      packetBufferCurrent++;
      packetBufferSize++;
      if(packetBufferSize >= 32 || currentChar == 4) {
        bufferFull = 1;
        packetBufferCurrent = 0;
      }
    }
  }
  while(bufferFull == 1) { 
    currentChar = packetBuffer[packetBufferCurrent];
    input.write(currentChar);
    packetBufferCurrent++;
    if(packetBufferCurrent == packetBufferSize) { // End of Transmission
      bufferFull = 0;
      packetBufferCurrent = 0;
      packetBufferSize = 0;
    }
  }*/
}*/