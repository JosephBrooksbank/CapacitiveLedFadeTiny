#ifndef COMMANDREADER_H
#define COMMANDREADER_H

#include "Arduino.h"
class CommandReader {
public:
    explicit CommandReader(volatile byte* buffer) : buffer(buffer) {};
    void parseNewMessage();
private:
    volatile byte* buffer;
};

#endif