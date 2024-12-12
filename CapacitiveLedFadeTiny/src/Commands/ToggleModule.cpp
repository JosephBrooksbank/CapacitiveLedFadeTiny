#include "commands.h"

void ToggleModule(uint8_t moduleAddress) {
    context.enabledModules.set(moduleAddress, true);
    for (uint8_t i = 0; i < context.timingGroupCount; i++) {
        context.timingGroups[i].checkOn(moduleAddress);
    }
}

void UnToggleModule(uint8_t moduleAddress) {
    context.enabledModules.set(moduleAddress, false);
    for (uint8_t i = 0; i < context.timingGroupCount; i++) {
        context.timingGroups[i].checkOff();
    }
}