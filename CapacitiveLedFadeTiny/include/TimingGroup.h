#ifndef TIMINGGROUP_H
#define TIMINGGROUP_H
#include "Arduino.h"
#include "AddressMap.h"

class TimingGroup {
public:
    TimingGroup() : addresses(AddressMap()), delay(0) {};
    TimingGroup(AddressMap addresses, uint16_t delay) : addresses(addresses), delay(delay) {};
    TimingGroup(uint8_t address, uint16_t delay) : addresses(AddressMap()), delay(delay) {
        addresses.set(address, true);
    };
    // turn on when any address in the group is enabled
    void checkOn(uint8_t address);
    // turn off when all the addresses in the group are disabled (from context)
    void checkOff();
    bool isEnabled();
    void tick();
    AddressMap addresses;
    uint16_t delay;
private:
    void trySetCounterOn();
    void trySetCounterOff();
    volatile uint16_t counterOn = 0 ;
    volatile uint16_t counterOff = 0;
    bool enabled = false;

};

#endif