#include "commands.h"
#include "context.h"

void SetTimingGroup(uint8_t groupIndex, AddressMap addresses, uint16_t delay) {
    if (groupIndex >= MAX_TIMING_GROUPS) {
        context.led->flashColor(CRGB::Red);
        return;
    }
    context.timingGroups[groupIndex] = TimingGroup(addresses, delay);
}
