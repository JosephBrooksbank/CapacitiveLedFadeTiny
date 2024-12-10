#include "commands.h"

void SetMode(byte mode) {
    switch (mode) {
        case Mode::NORMAL:
            context.mode = NORMAL;
            break;
        case Mode::RIPPLE:
            context.mode = RIPPLE;
            break;
        case Mode::ON:
            context.mode = ON;
            break;
        case Mode::CONFIG:
            context.mode = CONFIG;
            break;
        default:
            context.mode = UNKNOWN;
            break;
    }
}