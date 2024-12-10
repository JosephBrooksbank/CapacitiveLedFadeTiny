#ifndef ADDRESSMAP_H
#define ADDRESSMAP_H
#include "Arduino.h"
#include "config.h"


class AddressMap {
private:
    static constexpr uint8_t BitsPerElement = 32;
    static constexpr uint8_t NumElements = (NUM_MODULES + BitsPerElement - 1) / BitsPerElement;
    uint32_t map[NumElements]{0};

public:
    void set(uint8_t address, bool state);

    bool get(uint8_t address);

    void clear();
    bool IsAnyOn();

};

#endif