/*
    MIPS Library for easy coding of modules
*/

#ifndef MIPSLIB_H
#define MIPSLIB_H

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
        //Essential
        MipsLab();
        int MipsStart();
        int Start();

        //Module specific functions
        int ServoUp(uint32_t address, uint8_t intensity = 5);
        int ServoDown(uint32_t address, uint8_t intensity = 5);
        int ServoTo(uint32_t address, uint8_t angle);

        //Controller specific functions
        int ControlLoop(); // Used in setup if controller used

        //Controller specific module functions
        int ControlServoUp(std::string button, uint32_t address, uint8_t intensity = 5);
        int ControlServoDown(std::string button, uint32_t address, uint8_t intensity = 5);
        int ControlServoTo(std::string button, uint32_t address, uint8_t angle);

    private:
        //Containers
        static const std::map<std::string, uint16_t> controllerAddress;
        std::map<uint16_t, std::function<int(uint32_t, uint8_t*, uint8_t)>> controllerPair;
        std::vector<MipsModule> modules;

        //Essential
        int sendACT(uint32_t address, uint8_t* msg, uint8_t msgLen);
        std::string Error(uint16_t ecode);
};

#endif //MIPSLIB_H