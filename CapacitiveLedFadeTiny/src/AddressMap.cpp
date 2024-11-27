#include "AddressMap.hpp"
#include "config.h"

AddressMap::AddressMap(uint8_t address) : homePosition(findPosition(address)) {

}

AddressMap::AddressMap() : homePosition(findPosition(I2C_ADDRESS)) {

}


Position AddressMap::findPosition(uint8_t address) {
    for (int8_t row = 0; row < NUM_ROWS; row++) {
        for (int8_t col = 0; col < NUM_COLS; col++) {
            if (ADDRESS_MAP[row][col] == address) {
                return Position{row, col};
            }
        }
    }
    return Position{-1, -1};
}

bool AddressMap::isNeighbor(uint8_t address) {
    Position target = findPosition(address);
    if (target.row == -1) {
        return false;
    }
    return (homePosition.row == target.row && abs(homePosition.col - target.col) == 1) ||
           (homePosition.col == target.col && abs(homePosition.row - target.row) == 1);
}