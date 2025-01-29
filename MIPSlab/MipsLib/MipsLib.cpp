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
int MipsLab::sendACT(uint32_t address, uint8_t *msg, uint8_t msgLen) {
    uint8_t *b = (uint8_t *)&address; //Turn address into array of bytes
    Serial3.write(6+msgLen); // 6 is length of ACT packet
    Serial3.write(0x25); //0x25 is type ID for ACT packet, 2
    Serial3.write(b[3]); //address, 3
    Serial3.write(b[2]); //address, 4
    Serial3.write(b[1]); //address, 5
    Serial3.write(b[0]); //address, 6
    for (uint8_t i = 0; i < msgLen; i++) {
        Serial3.write(msg[i]); // message, 7+
    }
    return 1;
}

std::string MipsLab::Error(uint16_t ecode) {
    switch(ecode) {
        case 1:
            return "Servo has a maximum angle of 180 degrees";
        case 2:
            return "Intensity ranges from 0 to 9";
    }
}

//*  Public Essential
// Initialize some settings eventually
MipsLab::MipsLab() {
    //  Temporary direct assignment of connected modules
    modules.push_back({7,3,1}); // Module 7 is servo, first 
    modules.push_back({6,3,1}); // Module 6 is servo, second
    modules.push_back({5,2,1}); // Module 5 is claw,  third

    //  Temporary direct assignment of controller pairs
    controllerPair[12] = sendACT(6,(uint8_t*){245}, 1); // 1 button, base counter
    controllerPair[8]  = sendACT(6,(uint8_t*){235}, 1); // 4 button, base clockwise
    controllerPair[94] = sendACT(5,(uint8_t*){245}, 1); // 3 button, claw close
    controllerPair[90] = sendACT(5,(uint8_t*){235}, 1); // 6 button, claw open
    controllerPair[82] = sendACT(7,(uint8_t*){245}, 1); // 8 button, arm down
    controllerPair[74] = sendACT(7,(uint8_t*){235}, 1); // 9 button, arm up
}

// Should be put in the setup
int MipsLab::Start() {
    Serial3.begin(38400, SERIAL_8N2); // Hub Input / Output

    Serial.begin(38400); // Debugging
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
int MipsLab::ServoUp(uint32_t address, uint8_t intensity) {
    if(intensity > 9)
        return 2;
    sendACT(address,230+intensity);
}
int MipsLab::ControlServoUp(std::string button, uint32_t address, uint8_t intensity) {
    if(intensity > 9)
        return 2;
    controllerPair[controllerAddress[button]] = sendACT(address,(uint8_t*){230+intensity}, 1);
}
// Increases servo duty cycle, on "angle" of 240 to 249; higher angle increases distance
int MipsLab::ServoDown(uint32_t address, uint8_t intensity) {
    if(intensity > 9)
        return 2;
    sendACT(address,240+intensity);
}
int MipsLab::ControlServoDown(std::string button, uint32_t address, uint8_t intensity) {
    if(intensity > 9)
        return 2;
    controllerPair[controllerAddress[button]] = sendACT(address,(uint8_t*){240+intensity}, 1);
}
// Should go near enough
int ServoTo(uint32_t address, uint8_t angle) {
    if(angle > 180)
        return 1;
    sendACT(address,angle);
    return 0;
}
int ControlServoTo(std::string button, uint32_t address, uint8_t angle) {
    if(angle > 180)
        return 1;
    controllerPair[controllerAddress[button]] = sendACT(address,(uint8_t*){angle}, 1);
    return 0;
}

//* Public Controller Functions
int MipsLab::ControlLoop() {
    if (IrReceiver.decode() && IrReceiver.decodedIRData.protocol != UNKNOWN) {
        for(const auto &conPair : controllerPair) {
            if(IrReceiver.decodedIRData.command == conPair.first) {
                sendACT(conPair.second.address,conPair.second.msg);
                break;
            }
        }
    }
    IrReceiver.resume();

    if(Serial3.available() > 1) {
        Serial.println(Serial3.read());
    } 
    return 1;
}

#endif //MIPSLAB_CPP