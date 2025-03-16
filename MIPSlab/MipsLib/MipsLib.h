/*
    MIPS Library for easy coding of modules
*/

#ifndef MIPSLIB_H
#define MIPSLIB_H

#include "Arduino.h"

struct MipsModule {
    uint32_t address;
    uint16_t type;
    uint16_t version;
};
class MipsVector {
    MipsModule *array;
    uint16_t length;
    uint16_t capacity;

public:
    MipsVector() {
        array = new MipsModule[4];
        length = 0;
        capacity = 4;
    }
    ~MipsVector() { delete[] array; }
    void push_back(MipsModule newMod) {
        if (length >= capacity) {
            MipsModule *newArray = new MipsModule[capacity + 4];
            for (int i = 0; i < capacity; i++) {
                newArray[i] = array[i];
            }

            capacity += 4;
            delete[] array;
            array = newArray;
        }

        array[length] = newMod;
        length++;
    }
    MipsModule get(int index) {
        if (index >= length) return;
        return array[index];
    }
    uint16_t size() {
        return length;
    }
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
        MipsVector modules;
        static const char *moduleTypes[];

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
        char *Error(uint16_t ecode);

        //Constants
        const uint8_t IRPin = 5;
        const uint16_t BaudRate = 38400;
        const uint16_t UIBaudRate = 9600;
};

#endif //MIPSLIB_H