#include "CommandReader.h"
#include "commands.h"

void CommandReader::parseNewMessage() {
    char command = buffer[0];
    switch (command) {
        case 'm':
            SetMode(buffer[1]);
            break;
        case 't':
            ToggleModule(buffer[1]);
            break;
        case 'u':
            UnToggleModule(buffer[1]);
            break;
        default:
            break;
    }

}