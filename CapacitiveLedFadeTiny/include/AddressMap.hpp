#ifndef ADDRESSMAP_H
#define ADDRESSMAP_H

#include <Arduino.h>
#include "config.h"
struct Position {
    int8_t row = -1;
    int8_t col = -1;
    Position(int8_t row, int8_t col) : row(row), col(col) {}
};

class AddressMap {
public:
    explicit AddressMap(uint8_t address);
    AddressMap();
    Position findPosition(uint8_t address);
    bool isNeighbor(uint8_t address);
    Position homePosition;
private:
};
#endif