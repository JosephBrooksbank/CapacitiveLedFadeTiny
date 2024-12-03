#ifndef ADDRESSMAP_H
#define ADDRESSMAP_H

#include <Arduino.h>
#include "config.h"
struct Position {
    int8_t row = -1;
    int8_t col = -1;
    Position(int8_t row, int8_t col) : row(row), col(col) {}
};

struct Neighbors {
    unsigned up:1;
    unsigned down:1;
    unsigned left:1;
    unsigned right:1;
    unsigned upLeft:1;
    unsigned upRight:1;
    unsigned downLeft:1;
    unsigned downRight:1;

    operator uint8_t() {
        return *reinterpret_cast<uint8_t *>(this);
    }

    Neighbors& operator |= (uint8_t other) {
         *reinterpret_cast<uint8_t *>(this) |= other;
         return *this;
    }

    Neighbors& operator &= (uint8_t other) {
         *reinterpret_cast<uint8_t *>(this) &= other;
         return *this;
    }
};

class AddressMap {
public:
    explicit AddressMap(uint8_t address);
    AddressMap();
    Position findPosition(uint8_t address);
    Neighbors isNeighbor(uint8_t address);
    Position homePosition;
private:
};
#endif