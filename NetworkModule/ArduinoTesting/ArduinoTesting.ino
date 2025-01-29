#include <IRremote.hpp>

int flag = 1;
int packetLen = 0;
unsigned long lastHub = 0;

unsigned char buffer[256];
uint8_t bufferReadIndex = 0;
uint8_t bufferWriteIndex = 0;
uint8_t workingLen = 0;

struct Module {
  uint32_t address;
  uint16_t type;
  uint16_t version;
};

Module mod1 = {7,3,1};
Module mod2 = {6,3,1};
Module mod3 = {5,3,1};

Module modules[10];
uint8_t modulesLen;

void setup() {
  Serial.begin(38400);
  Serial3.begin(38400, SERIAL_8N2);

  lastHub = millis();
  modules[0] = mod1;
  modules[1] = mod2;
  modules[2] = mod3;
  modulesLen = 3;

  IrReceiver.begin(5);
}

void sendACT(uint32_t address, String comm) {
  uint8_t *b = (uint8_t *)&address;
  Serial3.write(7);
  Serial3.write(0x25);
  Serial3.write(b[3]);
  Serial3.write(b[2]);
  Serial3.write(b[1]);
  Serial3.write(b[0]);
  int type = 0;
  int num = comm.toInt();
  uint8_t *ang = (uint8_t *)&num;
  for (int i = 0; i < modulesLen; i++) {
    if (modules[i].address == address)
      type = modules[i].type;
  }

  switch (type) {
    case 1:
      if (comm == "on")
        Serial3.write(1);
      else
        Serial3.write(2);
      break;
    case 2:
    case 3:
      Serial3.write(ang[0]);
      break;
    default:
      break;
  }
}

void sendHUB() {
  Serial3.write(2);
  Serial3.write(0x9);
}

void recHUB() {
  uint32_t address;
  uint16_t type, version;
  bufferReadIndex+=2;
  address = (buffer[bufferReadIndex]<<3) + (buffer[bufferReadIndex+1]<<2) + (buffer[bufferReadIndex+2]<<1) + (buffer[bufferReadIndex+3]);
  type = (buffer[bufferReadIndex+4]<<1) + (buffer[bufferReadIndex+5]);
  version = (buffer[bufferReadIndex+6]<<1) + (buffer[bufferReadIndex+7]); 
  modules[modulesLen++] = {address,type,version};
  bufferReadIndex+=8;
}

char read() {
  uint8_t oldIndex = bufferReadIndex;
  bufferReadIndex+=1;
  return buffer[oldIndex];
} 

void checkBuffer() {
    uint8_t diff = bufferWriteIndex >= bufferReadIndex ? bufferWriteIndex - bufferReadIndex : bufferWriteIndex + (256 - bufferReadIndex);
    if(bufferReadIndex == bufferWriteIndex) return; // maybe enable sleep mode?
    if(buffer[bufferReadIndex] < 2) {
        bufferReadIndex+=1;
        return;
    }
    if(diff < buffer[bufferReadIndex]) return;
    switch (buffer[bufferReadIndex+1]) {
        case 0x18:
            recHUB();
            break;
    }

}

void loop() {
  if (IrReceiver.decode() && IrReceiver.decodedIRData.protocol != UNKNOWN) {
    Serial.println(IrReceiver.decodedIRData.command);
    switch(IrReceiver.decodedIRData.command) {
      case 0x5E:
        //sendACT(2, 245);
        sendACT(5, 245);
        break;
      case 0x5A:
        //sendACT(2, 235);
        sendACT(5, 235);
        break;
      case 0xC:
        //sendACT(1, 245);
        sendACT(6, 245);
        break;
      case 0x8:
        //sendACT(1, 235);
        sendACT(6, 235);
        break;
      case 0x52:
        //sendACT(3, 245);
        sendACT(7, 245);
        break;
      case 0x4A:
        //sendACT(3, 235);
        sendACT(7, 235);
        break;
    }
  }
  IrReceiver.resume();

  if(Serial3.available() > 1) {
    Serial.println(Serial3.read());
  }


/*
  //recieved packet to hub
  if(Serial3.available() > 1) {
    buffer[bufferWriteIndex] = Serial3.read();
    int g = buffer[bufferWriteIndex];
    Serial.println(g);
    bufferWriteIndex+=1;
  }
  checkBuffer();
  //every 5 second send HUB packet
  if(millis() - lastHub > 5000) {
    Serial.println("End");
    modulesLen = 0;
    sendHUB();
    lastHub = millis();
  }
  */
}
