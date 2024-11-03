#include "I2CEchoHandler.h"
#include "Wire.h"
#include "config.h"
#include "Serial.h"


bool isNewMessage = false;

I2CEchoHandler::I2CEchoHandler() {
    if (!instance) {
        instance = this;
    }
}


void I2CEchoHandler::setup() {
    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(receiveEventWrapper);
    Wire.onRequest(requestEventWrapper);
}

void I2CEchoHandler::onRequest() {
    SERIAL_PRINT("sending via i2c: ");
    for (uint8_t i = 0; i < messageLength; i++) {
        Wire.write(buffer[i]);
        SERIAL_PRINT(buffer[i]);
    }
    SERIAL_PRINTLN();
}

void I2CEchoHandler::onReceive(int bytesReceived) {
    if (bytesReceived > BUFFER_SIZE) {
        return;
    }

    for (auto i = 0; i < bytesReceived; i++) {
        buffer[i] = Wire.read();
        SERIAL_PRINTLN(buffer[i]);
    }
    messageLength = bytesReceived;
    isNewMessage = true;
}

void I2CEchoHandler::receiveEventWrapper(int bytesReceived) {

    if (instance) {
        instance ->onReceive(bytesReceived);
    }
}

void I2CEchoHandler::requestEventWrapper() {
    if (instance) {
        instance->onRequest();
    }
}


I2CEchoHandler *I2CEchoHandler::instance = nullptr;
