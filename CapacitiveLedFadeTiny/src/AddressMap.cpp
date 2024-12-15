#include "AddressMap.h"

void AddressMap::clear() {
    for (uint8_t i = 0; i < NumElements; i++) {
        map[i] = 0;
    }
}

bool AddressMap::IsAnySet() {
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


AddressMap AddressMap::clone() {
    AddressMap newMap;
    for (uint8_t i = 0; i < NumElements; i++) {
        newMap.map[i] = map[i];
    }
    return newMap;
}

bool AddressMap::hasMatch(AddressMap &other) {
    for (uint8_t i = 0; i < NumElements; i++) {
        if (map[i] & other.map[i]) {
            return true;
        }
    }
    return false;
}

AddressMap AddressMap::fromBuffer(volatile byte *bytes, uint8_t startingIndex) {
    AddressMap newMap;
    uint8_t index = 0;
    for (uint8_t i = startingIndex; i < NumElements; i++) {
        newMap.map[index++] = bytes[i];
    }
    return newMap;
}