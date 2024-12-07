#include "Capacitive.h"
#include "ADCTouch.h"
#include "context.h"

void Capacitive::init() {
    reference = ADCTouch.read(pin, 500);
}

void Capacitive::setReference() {
    reference = ADCTouch.read(pin, 500);
}

void Capacitive::setSensitivity(uint8_t sensitivity) {
    context.capacitive_sensitivity = sensitivity;
}

bool Capacitive::isTouched() {
    int value = ADCTouch.read(pin, 70);
    if( value - reference > context.capacitive_sensitivity) {
        if (cyclesTouched < 255) {
            cyclesTouched++;
        }
        if (cyclesTouched > context.capacitive_debounce) {
            return true;
            state = TOUCHED;
        }
    }
    state = NOT_TOUCHED;
    return false;
}

void Capacitive::tick() {
    if (state == TOUCHED) {
        if (context.capacitive_msTouched < 0xFFFFFFFF) {
            context.capacitive_msTouched++;
        }
    } else {
        context.capacitive_msTouched = 0;
    }
}



