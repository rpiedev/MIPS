/*
    MIPS Library for easy coding of modules
*/

#ifndef MIPSLIB_CPP
#define MIPSLIB_CPP

#include "Arduino.h"
#include "MipsLib.h"

//*  Private Containers
const char *MipsLab::moduleTypes[] = {"","LED","Claw","Elbow"};

//*  Private Essential
//sends ACT packet from Hub
int MipsLab::sendACT(uint32_t address, const uint32_t msg, uint8_t msgLen) {
    uint8_t *b = (uint8_t *)&address; //Turn address into array of bytes
    uint8_t *m = (uint8_t *)&msg; //Turn message into array of bytes
    Serial1.write(6+msgLen); // 6 is length of ACT packet
    Serial1.write(0x25); //0x25 is type ID for ACT packet, 2
    Serial1.write(b[3]); //address, 3
    Serial1.write(b[2]); //address, 4
    Serial1.write(b[1]); //address, 5
    Serial1.write(b[0]); //address, 6
    for (uint8_t i = 0; i < msgLen; i++) {
        Serial1.write(m[i]); // message, 7+
    }
    return 1;
}

int MipsLab::sendHUB() {
    Serial1.write(0x2);
    Serial1.write(0x9);
    return 1;
}

int MipsLab::updateModules() {
    uint32_t isDoneTimer = 0;
    sendHUB(); // send hub to see what modules are connected
    while(isDoneTimer < 100000) { //this one gets all the messages hopefully (imprecise)
        
        if(Serial1.available() > 0) {
            write(Serial1.read());
            isDoneTimer = 0;
        }
        checkBuffer();
        isDoneTimer++;
    }
    
    for(int i=0;i<30;i++) {
        Serial.println();
    }
    isDoneTimer = 1;
    Serial.print("Modules connected... ");
    Serial.println(modules.size());
    for (int i = 0; i < modules.size(); i++) {
        Serial.print(isDoneTimer++);
        Serial.print(": ");
        Serial.print(moduleTypes[modules.get(i).type]);
        Serial.print(" module with address ");
        Serial.println(modules.get(i).address);
    }

    // await for end of serial1 messages
    // once that is done do it again to confirm, until same result twice in a row
    // afterwards, print to serial results
    // continue
    return 1;
}

char *MipsLab::Error(uint16_t ecode) {
    switch(ecode) {
        case 1:
            return "Servo has a maximum angle of 180 degrees";
        case 2:
            return "Intensity ranges from 0 to 9";
        case 4:
            return "Module is not correct type";
        default:
            return "Undefined Error";
    }
}
//Private Buffer
unsigned char MipsLab::read() {
    uint8_t oldIndex = bufferReadIndex;
    bufferReadIndex+=1;
    return buffer[oldIndex];
}
void MipsLab::write(unsigned char in) {
    buffer[bufferWriteIndex] = in;
    bufferWriteIndex+=1;
}

void memcpy(void *dest, const void *src) {
    ((unsigned char *)dest)[0] = ((unsigned char *)src)[0];
    return;
}

void MipsLab::checkBuffer() {
    uint8_t diff = bufferWriteIndex >= bufferReadIndex ? bufferWriteIndex - bufferReadIndex : bufferWriteIndex + (64 - bufferReadIndex);
    if(bufferReadIndex == bufferWriteIndex) return; // maybe enable sleep mode?
    if(buffer[bufferReadIndex] < 2) {
        bufferReadIndex+=1;
        return;
    }
    if(diff < buffer[bufferReadIndex]) return;
    unsigned char packetLen = read();
    unsigned char packetType = read();
    if(packetType!=0x18) {
        bufferReadIndex += (packetLen-2);
        return;
    }
    MipsModule newMod;
    memcpy((char*)&newMod.address, buffer+bufferReadIndex+3);
    memcpy((char*)&newMod.address+1, buffer+bufferReadIndex+2);
    memcpy((char*)&newMod.address+2, buffer+bufferReadIndex+1);
    memcpy((char*)&newMod.address+3, buffer+bufferReadIndex);
    memcpy((char*)&newMod.type, buffer+bufferReadIndex+5);
    memcpy((char*)&newMod.type+1, buffer+bufferReadIndex+4);
    memcpy((char*)&newMod.version, buffer+bufferReadIndex+7);
    memcpy((char*)&newMod.version+1, buffer+bufferReadIndex+6);
    bufferReadIndex += 8;
    
    modules.push_back(newMod);
}

//*  Public 
// Initialize some settings eventually
MipsLab::MipsLab() {

}

// Should be put in the setup
int MipsLab::Start() {
    Serial1.begin(BaudRate, SERIAL_8N2); // Hub Input / Output (packets)
    Serial.begin(BaudRate); // UI

    while (!Serial) {} // Wait for the serial to start before continuing

    updateModules();

    return 1;
}


//*  Public Module Functions with Controller versions
//TODO These assume the address actually goes to a servo, which should be changed to check
// Lowers servo duty cycle, on "angle" of 230 to 239; higher angle increases distance
int MipsLab::ElbowUp(uint32_t address, uint8_t intensity) {
    if(intensity > 9)
        return 2;
    const uint32_t msg = 230+intensity;
    sendACT(address, msg, 1);
    return 0;
}
// Increases Elbow duty cycle, on "angle" of 240 to 249; higher angle increases distance
int MipsLab::ElbowDown(uint32_t address, uint8_t intensity) {
    if(intensity > 9)
        return 2;
    const uint32_t msg = 240+intensity;
    sendACT(address, msg, 1);
    return 0;
}
// Should go near enough
int MipsLab::ElbowTo(uint32_t address, uint8_t angle) {
    if(angle > 180)
        return 1;
    const uint32_t msg = angle;
    sendACT(address, msg, 1);
    return 0;
}

#endif //MIPSLAB_CPP