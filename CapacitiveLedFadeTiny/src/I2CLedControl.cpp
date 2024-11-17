#include "I2CLedControl.hpp"
#include "Wire.h"
#include "config.h"
#include "Serial.h"


I2CLedControl::I2CLedControl(volatile int8_t & touchedCounter) : command('r'), isNewCommand(false), touchedCounter(touchedCounter) {
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

void I2CLedControl::onRequest() {
    Wire.write(touchedCounter);
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
        SERIAL_PRINT("setting buffer to ");
        SERIAL_PRINTLN(buffer[i]);
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


I2CLedControl *I2CLedControl::instance = nullptr;