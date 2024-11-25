#include <Wire.h>
#include "I2CEventHandler.hpp"

byte* I2CEventHandler::getBuffer() {
    return buffer;
}

void I2CEventHandler::writeInt(int value) {
    Wire.write((byte)(value >> 8));
    Wire.write((byte)value);
}
