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

    private:
        //Containers
        std::vector<MipsModule> modules;
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
        const uint8_t IRPin = 5;
        const uint16_t BaudRate = 38400;
        const uint16_t UIBaudRate = 9600;
};

#endif //MIPSLIB_H