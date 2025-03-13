/*
    MIPS Library for easy coding of modules
*/

#ifndef MIPSLIB_H
#define MIPSLIB_H

#include "Arduino.h"
#include <Arduino_AVRSTL.h>
#include <string>
#include <cstring>
#include <vector>

struct MipsModule {
    uint32_t address;
    uint16_t type;
    uint16_t version;
};
struct ControllerMessage {
    uint16_t controllerAddress;
    uint32_t moduleAddress;
    uint32_t msg;
    uint8_t msgLen;
};
struct ControllerAddress {
    std::string name;
    uint8_t address;
};

class MipsLab {
    public:
        //Essential
        MipsLab();
        int Start(); // Must be used in setup
        int Loop(); // Must be used in loop

        //TODO change address name to ID, its easier to understand
        //Module specific functions
        int ElbowUp(uint32_t address, uint8_t intensity = 5);
        int ElbowDown(uint32_t address, uint8_t intensity = 5);
        int ElbowTo(uint32_t address, uint8_t angle);

        //Controller specific functions
        int ControlStart(); // Used in setup if controller is used
        int ControlLoop(); // Used in loop if controller is used

        //Controller specific module functions
        int ControlElbowUp(std::string button, uint32_t address, uint8_t intensity = 5);
        int ControlElbowDown(std::string button, uint32_t address, uint8_t intensity = 5);
        int ControlElbowTo(std::string button, uint32_t address, uint8_t angle);

    private:
        //Containers
        std::vector<ControllerMessage> controllerMessages;
        void newControllerMessage(ControllerMessage cm);

        std::vector<MipsModule> modules;

        static const ControllerAddress controllerAddresses[];
        uint16_t getControllerAddress(std::string name);

        static const std::string moduleTypes[];

        //Circular Buffer
        unsigned char buffer[64];
        uint8_t bufferReadIndex = 0;
        uint8_t bufferWriteIndex = 0;
        uint8_t workingLen = 0;
        unsigned char read();
        void write(unsigned char in);
        void checkBuffer();

        //Essential
        int sendACT(uint32_t address, const uint32_t msg, uint8_t msgLen);
        int sendHUB();
        int updateModules();
        std::string Error(uint16_t ecode);

        //Constants
        const uint8_t IRPin = 17;
        const uint16_t BaudRate = 38400;
        const uint16_t UIBaudRate = 9600;
};

#endif //MIPSLIB_H