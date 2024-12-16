#include "CommandReader.h"
#include "commands.h"
#include "config.h"

void CommandReader::parseNewMessage() {
    switch (context.mode) {
        case RIPPLE:
        case ON:
        case UNKNOWN:
        case NORMAL:
            normalModeCommands();
            break;
        case CONFIG:
            configModeCommands();
            break;
        default:
            break;
    }
}

void CommandReader::normalModeCommands() {
    if (DEBUG_LIGHTS) {
        context.led->flashColor(CRGB::Red);
    }
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

void CommandReader::configModeCommands() {
    char command = buffer[0];
    switch (command) {
        case 'm':
            SetMode(buffer[1]);
            break;
        case 'g': {
            uint8_t groupIndex = buffer[1];
            uint16_t delay = buffer[2] << 8 | buffer[3];
            AddressMap addresses =AddressMap::fromBuffer(buffer, 4);
            SetTimingGroup(0, addresses, 100);
            break;
        }
        default:
            break;
    }
}