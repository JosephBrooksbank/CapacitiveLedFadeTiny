#include "commands.h"

void ToggleModule(uint8_t moduleAddress) {
    context.enabledModules.set(moduleAddress, true);
}

void UnToggleModule(uint8_t moduleAddress) {
    context.enabledModules.set(moduleAddress, false);
}