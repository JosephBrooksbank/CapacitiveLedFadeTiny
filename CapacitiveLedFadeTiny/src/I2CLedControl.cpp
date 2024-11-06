#include "I2CLedControl.hpp"
#include "Wire.h"
#include "config.h"
#include "Serial.h"


I2CLedControl::I2CLedControl(int& isTouched) : command('r'), isNewCommand(false), touchedValue(isTouched) {
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
//    SERIAL_PRINT("sending via i2c: ");
//    int test = 43;
    Wire.write(buffer[0]);
    Wire.write(buffer[1]);
    Wire.write(buffer[2]);
//    SERIAL_PRINT(buffer[0]);
//    if (buffer[0] == 'r') {
//        SERIAL_PRINT(" command: status: ");
//        SERIAL_PRINTLN(isTouched);
//        Wire.write(isTouched);
//    } else if (buffer[0] == 'm') {
//        SERIAL_PRINTLN(" command: message");
//        Wire.write("message");
//    }
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