#ifndef CONTEXT_H
#define CONTEXT_H
#include "Arduino.h"

enum LEDState {
    OFF,
    FADE_ON,
    HOLD,
    FADE_OFF
};

struct Context {
    uint8_t led_fadeOnSpeed;
    uint8_t led_fadeOffSpeed;
    uint8_t led_holdTime;
    LEDState led_state;
    uint8_t capacitive_sensitivity;
    uint8_t capacitive_debounce;
    volatile uint32_t capacitive_msTouched;

};

static Context context {
    .led_fadeOnSpeed = 2,
    .led_fadeOffSpeed = 2,
    .led_holdTime = 10,
    .led_state = OFF,
    .capacitive_sensitivity = 5,
    .capacitive_debounce = 5,
    .capacitive_msTouched = 0
};

#endif