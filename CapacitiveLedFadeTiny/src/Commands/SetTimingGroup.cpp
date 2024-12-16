#include "commands.h"
#include "context.h"

void SetTimingGroup(volatile byte* buffer, uint8_t index) {
    uint8_t groupIndex = buffer[index++];
    uint16_t delay = buffer[index] << 8 | buffer[index+1];
    index += 2;

    if (groupIndex >= MAX_TIMING_GROUPS) {
        context.led->flashColor(CRGB::Red);
        return;
    }
    context.timingGroups[groupIndex].addresses.fromBuffer(buffer, index);
    context.timingGroups[groupIndex].delay = delay;
}
