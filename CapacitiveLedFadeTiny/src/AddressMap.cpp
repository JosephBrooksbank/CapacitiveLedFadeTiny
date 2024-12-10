#include "AddressMap.h"

void AddressMap::clear() {
    for (uint8_t i = 0; i < NumElements; i++) {
        map[i] = 0;
    }
}

bool AddressMap::IsAnyOn() {
    for (uint8_t i = 0; i < NumElements; i++) {
        if (map[i] != 0) {
            return true;
        }
    }
    return false;
}

void AddressMap::set(uint8_t address, bool state) {
    uint8_t element = address / BitsPerElement;
    uint8_t bit = address % BitsPerElement;
    if (state) {
        map[element] |= 1 << bit;
    } else {
        map[element] &= ~(1 << bit);
    }
}

bool AddressMap::get(uint8_t address) {
    uint8_t element = address / BitsPerElement;
    uint8_t bit = address % BitsPerElement;
    return (map[element] >> bit) & 1;
}

