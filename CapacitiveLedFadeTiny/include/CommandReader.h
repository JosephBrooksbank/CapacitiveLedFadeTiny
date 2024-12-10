#ifndef COMMANDREADER_H
#define COMMANDREADER_H

#include "Arduino.h"
class CommandReader {
public:
    explicit CommandReader(volatile byte* buffer) : buffer(buffer) {};
    void parseNewMessage();
private:
    void normalModeCommands();
    void configModeCommands();
    volatile byte* buffer;
};

#endif