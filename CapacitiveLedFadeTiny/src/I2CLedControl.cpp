#include "I2CLedControl.hpp"
#include "Wire.h"
#include "config.h"


I2CLedControl::I2CLedControl(volatile int8_t & touchedCounter, int& capacitiveValue, int& capacitiveReference) :
command('r'),
isNewCommand(false),
touchedCounter(touchedCounter),
capacitiveValue(capacitiveValue),
capacitiveReference(capacitiveReference)
{
    if (!instance) {
        instance = this;
    }
    buffer[0] = 121;
    buffer[1] = 121;
    buffer[2] = 121;
}


void I2CLedControl::setup() {
    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(receiveEventWrapper);
    Wire.onRequest(requestEventWrapper);
}

// first byte is how many 8ms increments the capacitive sensor has been touched
// next two bytes are the capacitive value, int

void I2CLedControl::onRequest() const {
    Wire.write(touchedCounter);
    writeInt(capacitiveValue);
    writeInt(capacitiveReference);
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