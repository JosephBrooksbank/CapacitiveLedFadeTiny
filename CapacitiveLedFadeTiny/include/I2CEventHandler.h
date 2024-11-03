//
// Created by joebr on 10/30/2024.
//

#ifndef CAPACITIVELEDFADETINY_I2CEVENTHANDLER_H
#define CAPACITIVELEDFADETINY_I2CEVENTHANDLER_H

#include "Arduino.h"

const uint8_t BUFFER_SIZE = 30;
class I2CEventHandler {

public:
    virtual ~I2CEventHandler() = default;
    virtual void onReceive(int bytesReceived) = 0;
    virtual void onRequest() = 0;
    virtual void setup() = 0;

protected:
    uint8_t messageLength = 0;
    char buffer[BUFFER_SIZE];
};


#endif //CAPACITIVELEDFADETINY_I2CEVENTHANDLER_H
