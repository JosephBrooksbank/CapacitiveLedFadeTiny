#ifndef CAPACITIVE_H
#define CAPACITIVE_H

#include "Arduino.h"

enum CapacitiveState {
    NOT_TOUCHED,
    TOUCHED
};

class Capacitive {
public:
    Capacitive(int pin) : pin(pin) {};
    void init();
    void setReference();
    void setSensitivity(uint8_t sensitivity);
    bool isTouched();
    void tick();

private:
    CapacitiveState state = NOT_TOUCHED;
    uint8_t cyclesTouched = 0;
    int pin;
    int reference;

};

#endif