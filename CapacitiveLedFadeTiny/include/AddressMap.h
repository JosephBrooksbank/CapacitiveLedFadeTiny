#ifndef ADDRESSMAP_H
#define ADDRESSMAP_H
#include "Arduino.h"
#include "config.h"


static constexpr uint8_t BitsPerElement = 8;
static constexpr uint8_t NumElements = (NUM_MODULES + BitsPerElement - 1) / BitsPerElement;
class AddressMap {
private:
    uint8_t map[NumElements]{0};

public:
    AddressMap() = default;
    void set(uint8_t address, bool state);

    bool get(uint8_t address);

    // returns true if any of the bits are set in both maps
    bool hasMatch(AddressMap &other);
    void clear();
    bool IsAnySet();
    AddressMap clone();
    static AddressMap fromBuffer(volatile byte *bytes, uint8_t startingIndex);

};

#endif