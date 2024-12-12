#ifndef ADDRESSMAP_H
#define ADDRESSMAP_H
#include "Arduino.h"
#include "config.h"


static constexpr uint8_t BitsPerElement = 32;
static constexpr uint8_t NumElements = (NUM_MODULES + BitsPerElement - 1) / BitsPerElement;
class AddressMap {
private:
    uint32_t map[NumElements]{0};

public:
    void set(uint8_t address, bool state);

    bool get(uint8_t address);

    // returns true if any of the bits are set in both maps
    bool hasMatch(AddressMap &other);
    void clear();
    bool IsAnySet();
    AddressMap clone();

};

#endif