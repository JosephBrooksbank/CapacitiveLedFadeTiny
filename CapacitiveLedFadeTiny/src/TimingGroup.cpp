#include "TimingGroup.h"
#include "context.h"

void TimingGroup::checkOn(uint8_t address) {
    if (addresses.get(address)) {
        trySetCounterOn();
    }
}

void TimingGroup::checkOff() {
    if (!context.enabledModules.hasMatch(addresses)) {
        trySetCounterOff();
    }
}

void TimingGroup::trySetCounterOn() {
    if (counterOn == 0 && !enabled) {
        counterOn = delay;
    }
}

void TimingGroup::trySetCounterOff() {
    if (counterOff == 0 && enabled) {
        counterOff = delay;
    }
}

bool TimingGroup::isEnabled() {
    return enabled;
}

void TimingGroup::tick() {
    if (counterOff > 0) {
        counterOff--;
        if (counterOff == 0) {
            enabled = false;
        }
    }
    if (counterOn > 0) {
        counterOn--;
        if (counterOn == 0) {
            enabled = true;
        }
    }
}