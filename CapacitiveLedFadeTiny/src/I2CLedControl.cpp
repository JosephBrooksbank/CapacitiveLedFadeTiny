#include "I2CLedControl.hpp"
#include "Wire.h"
#include "config.h"
#include "context.h"


I2CLedControl::I2CLedControl()
{
    if (!instance) {
        instance = this;
    }
}


void I2CLedControl::setup() {
    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(receiveEventWrapper);
    Wire.onRequest(requestEventWrapper);
}

void I2CLedControl::onRequest() const {
    for (uint16_t i = 0; i < sizeof (Context); i++) {
        Wire.write(((byte*)&context)[i]);
    }
}

void I2CLedControl::onReceive(int bytesReceived) {
    if (bytesReceived > BUFFER_SIZE) {
        return;
    }
    if (bytesReceived <= 0 ) {
        return;
    }

    isNewCommand = true;
    command = Wire.read();

    for (auto i = 0; i < bytesReceived-1; i++) {
        buffer[i] = Wire.read();
    }
    messageLength = bytesReceived;
}

void I2CLedControl::receiveEventWrapper(int bytesReceived) {

    if (instance) {
        instance ->onReceive(bytesReceived);
    }
}

void I2CLedControl::requestEventWrapper() {
    if (instance) {
        instance->onRequest();
    }
}

void I2CLedControl::writeInt(int value) {
    Wire.write((byte)(value >> 8));
    Wire.write((byte)value);
}

byte* I2CLedControl::getBuffer() {
    return buffer;
}

I2CLedControl *I2CLedControl::instance = nullptr;