#include "Arduino.h"
#include <IRremote.hpp>
#include "MipsLib.h"

const std::map<std::string, uint16_t> MipsLab::controllerAddress {
    {"CH-", 69}, {"CH",  70}, {"CH+", 71},
    {"PREV",68}, {"NEXT",64}, {"PLAY",67},
    {"VOL-", 7}, {"VOL+",21}, {"EQ",   9},
    {"0",   22}, {"100+",25}, {"200+",13},
    {"1",   12}, {"2",   24}, {"3",   94},
    {"4",    8}, {"5",   28}, {"6",   90},
    {"7",   66}, {"8",   82}, {"9",   74},
};

//
MipsLab::MipsLab() {
    //  Temporary direct assignment of connected modules
    modules.push_back({7,3,1}); // Module 7 is servo, first 
    modules.push_back({6,3,1}); // Module 6 is servo, second
    modules.push_back({5,2,1}); // Module 5 is claw,  third

    //  Temporary direct assignment of controller pairs
    controllerPair[12] = {6,245}; // 1 button, base counter
    controllerPair[8] =  {6,235}; // 4 button, base clockwise
    controllerPair[94] = {5,245}; // 3 button, claw close
    controllerPair[90] = {5,235}; // 6 button, claw open
    controllerPair[82] = {7,245}; // 8 button, arm down
    controllerPair[74] = {7,235}; // 9 button, arm up
}

// Should be put in the setup
int MipsLab::MipsStart() {
    Serial3.begin(38400, SERIAL_8N2); // Hub Input / Output

    Serial.begin(38400); // Debugging
    return 1;
}
// Should be put in the setup
int MipsLab::ControlStart() {
    IrReceiver.begin(5, ENABLE_LED_FEEDBACK); // IR reciever on pin 5
    return 1;
}

//sends ACT packet from Hub
int MipsLab::sendACT(uint32_t address, uint8_t msg) {
    uint8_t *b = (uint8_t *)&address; //Turn address into array of bytes
    Serial3.write(6+1); // 7 is length of ACT packet
    Serial3.write(0x25); //0x25 is type ID for ACT packet, 2
    Serial3.write(b[3]); //address, 3
    Serial3.write(b[2]); //address, 4
    Serial3.write(b[1]); //address, 5
    Serial3.write(b[0]); //address, 6
    //Message to be expanded to multiple bytes eventually
    Serial3.write(msg); //message, 7
    return 1;
}

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

//These assume the address actually goes to a servo, which should be changed to check
int MipsLab::ServoUp(uint32_t address) {
    sendACT(address,235);
}
int MipsLab::ServoDown(uint32_t address) {
    sendACT(address,245);
}