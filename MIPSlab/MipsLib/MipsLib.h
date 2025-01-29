/*
    MIPS Library for easy coding of modules
*/

#ifndef MipsLib_h
#define MipsLib_h

#include "Arduino.h"
#include <vector>
#include <map>
#include <string>

struct MipsModule {
    uint32_t address;
    uint16_t type;
    uint16_t version;
};
struct am {
    uint32_t address;
    uint8_t msg;
};

class MipsLab {
    public:
        MipsLab();
        int MipsStart();
        int ControlStart();
        int ControlLoop();

        //Module specific functions
        int ServoUp(uint32_t address);
        int ServoDown(uint32_t address);
    private:
        static const std::map<std::string, uint16_t> controllerAddress;
        int sendACT(uint32_t address, uint8_t msg);
        std::vector<MipsModule> modules;
        std::map<uint16_t, am> controllerPair;
};

#endif