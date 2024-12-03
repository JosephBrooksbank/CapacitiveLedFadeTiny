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

Neighbors AddressMap::isNeighbor(uint8_t address) {
    Position target = findPosition(address);
    Neighbors neighbors {false, false, false, false, false, false, false, false};
    if (target.row == -1) {
        return neighbors;
    }

    if (homePosition.row == target.row) {
        if (homePosition.col == target.col + 1) {
            neighbors.right = true;
        } else if (homePosition.col == target.col - 1) {
            neighbors.left = true;
        }
    }
    if (homePosition.col == target.col) {
        if (homePosition.row == target.row + 1) {
            neighbors.down = true;
        } else if (homePosition.row == target.row - 1) {
            neighbors.up = true;
        }
    }
    if (homePosition.row == target.row + 1) {
        if (homePosition.col == target.col + 1) {
            neighbors.downRight = true;
        } else if (homePosition.col == target.col - 1) {
            neighbors.downLeft = true;
        }
    }
    if (homePosition.row == target.row - 1) {
        if (homePosition.col == target.col + 1) {
            neighbors.upRight = true;
        } else if (homePosition.col == target.col - 1) {
            neighbors.upLeft = true;
        }
    }
    return neighbors;
}