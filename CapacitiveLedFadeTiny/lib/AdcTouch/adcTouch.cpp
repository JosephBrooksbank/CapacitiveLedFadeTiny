#include "Arduino.h"

// read the capacitive value of an analog pin.
// @param samples MUST be a power of 2.
int read(byte analogPin, int samples) {
    long _value = 0;
    for (int _counter = 0; _counter < samples; _counter++) {

        pinMode(analogPin, INPUT_PULLUP);
        uint8_t oldSREG = SREG;
        cli();

        analogRead(ADC_GROUND);
        pinMode(analogPin, INPUT);
        int16_t val = analogRead(analogPin);
        SREG = oldSREG;
        _value += val;
    }

    // right shifting to divide by 2 repeatedly
    while (samples > 1) {
        _value >>= 1;
        samples >>= 1;
    }

    return _value;
}
