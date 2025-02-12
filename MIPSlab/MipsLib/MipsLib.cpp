/*
    MIPS Library for easy coding of modules
*/

#ifndef MIPSLIB_CPP
#define MIPSLIB_CPP

#include "Arduino.h"
#include <IRremote.hpp>
#include "MipsLib.h"

//*  Private Containers
const std::map<std::string, uint16_t> MipsLab::controllerAddress {
    {"CH-", 69}, {"CH",  70}, {"CH+", 71},
    {"PREV",68}, {"NEXT",64}, {"PLAY",67},
    {"VOL-", 7}, {"VOL+",21}, {"EQ",   9},
    {"0",   22}, {"100+",25}, {"200+",13},
    {"1",   12}, {"2",   24}, {"3",   94},
    {"4",    8}, {"5",   28}, {"6",   90},
    {"7",   66}, {"8",   82}, {"9",   74},
};

//*  Private Essential
//sends ACT packet from Hub
int MipsLab::sendACT(uint32_t address, const uint32_t msg, uint8_t msgLen) {
    uint8_t *b = (uint8_t *)&address; //Turn address into array of bytes
    uint8_t *m = (uint8_t *)&msg; //Turn message into array of bytes
    Serial3.write(6+msgLen); // 6 is length of ACT packet
    Serial3.write(0x25); //0x25 is type ID for ACT packet, 2
    Serial3.write(b[3]); //address, 3
    Serial3.write(b[2]); //address, 4
    Serial3.write(b[1]); //address, 5
    Serial3.write(b[0]); //address, 6
    for (uint8_t i = 0; i < msgLen; i++) {
        Serial3.write(m[i]); // message, 7+
    }
    return 1;
}

int MipsLab::sendHUB() {
    Serial3.write(0x2);
    Serial3.write(0x9);
    return 1;
}

int MipsLab::updateModules() {
    uint32_t isDoneTimer = 0;
    sendHUB(); // send hub to see what modules are connected
    while(isDoneTimer < 100000) { //this one gets all the messages hopefully (imprecise)
        
        if(Serial3.available() > 0) {
            write(Serial3.read());
            isDoneTimer = 0;
        }
        checkBuffer();
        isDoneTimer++;
    }
    isDoneTimer = 0;
    for(auto const& module : modules) {
        isDoneTimer++;
        Serial.print(isDoneTimer);
        Serial.print(": Module with address ");
        Serial.print(module.first);
        Serial.print(" of type ");
        Serial.println(module.second.type);
    }
    // await for end of serial3 messages
    // once that is done do it again to confirm, until same result twice in a row
    // afterwards, print to serial results
    // continue
    return 1;
}

std::string MipsLab::Error(uint16_t ecode) {
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
    uint32_t address;
    MipsModule newMod;
    std::memcpy(&address, buffer+bufferReadIndex+3, 1);
    std::memcpy((char*)&address+1, buffer+bufferReadIndex+2, 1);
    std::memcpy((char*)&address+2, buffer+bufferReadIndex+1, 1);
    std::memcpy((char*)&address+3, buffer+bufferReadIndex, 1);
    std::memcpy((char*)&newMod.type, buffer+bufferReadIndex+5, 1);
    std::memcpy((char*)&newMod.type+1, buffer+bufferReadIndex+4, 1);
    std::memcpy((char*)&newMod.version, buffer+bufferReadIndex+7, 1);
    std::memcpy((char*)&newMod.version+1, buffer+bufferReadIndex+6, 1);
    
    bufferReadIndex += 8;
    modules.emplace(address, newMod);
}

//*  Public 
// Initialize some settings eventually
MipsLab::MipsLab() {
    /*  Temporary direct assignment of connected modules
    MipsModule servo = {3,1};
    MipsModule claw = {2,1};
    modules.emplace(7, servo); // Module 7 is servo, first 
    modules.emplace(6, servo); // Module 6 is servo, second
    modules.emplace(5, claw); // Module 5 is claw,  third
    */
}

// Should be put in the setup
int MipsLab::Start() {
    Serial3.begin(38400, SERIAL_8N2); // Hub Input / Output (packets)
    Serial.begin(38400); // UI

    updateModules();

    return 1;
}
// Should be put in the setup
int MipsLab::ControlStart() {
    IrReceiver.begin(5, ENABLE_LED_FEEDBACK); // IR reciever on pin 5
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
int MipsLab::ControlElbowUp(std::string button, uint32_t address, uint8_t intensity) {
    if(intensity > 9)
        return 2;
    const uint32_t msg = 230+intensity;
    am val = {address, msg, 1};
    controllerPair.emplace(controllerAddress.at(button), val);
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
int MipsLab::ControlElbowDown(std::string button, uint32_t address, uint8_t intensity) {
    if(intensity > 9)
        return 2;
    const uint32_t msg = 240+intensity;
    am val = {address, msg, 1};
    controllerPair.emplace(controllerAddress.at(button),val);
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
int MipsLab::ControlElbowTo(std::string button, uint32_t address, uint8_t angle) {
    if(angle > 180)
        return 1;
    const uint32_t msg = angle;
    am val = {address, msg, 1};
    controllerPair.emplace(controllerAddress.at(button),val);
    return 0;
}

//* Public Controller Functions
int MipsLab::ControlLoop() {
    if (IrReceiver.decode() && IrReceiver.decodedIRData.protocol != UNKNOWN) {    
        for(const auto &conPair : controllerPair) {
            if(IrReceiver.decodedIRData.command == conPair.first) {
                sendACT(conPair.second.address, conPair.second.msg, conPair.second.msgLen);
                break;
            }
        }
    }
    IrReceiver.resume();

    //if(Serial3.available() > 1) {
    //    Serial.println(Serial3.read());
    //} 
    return 1;
}

#endif //MIPSLAB_CPP