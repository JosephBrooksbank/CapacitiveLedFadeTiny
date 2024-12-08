#ifndef CONTEXT_H
#define CONTEXT_H
#include "Arduino.h"

enum LEDState {
    OFF,
    FADE_ON,
    HOLD,
    FADE_OFF
};

enum Animation {
    NONE,
    VALUE_INDICATE,
    RAINBOW
};

#pragma pack(push, 1) // Force 1-byte alignment so we can send the struct over I2C
struct Context {
    uint8_t led_fadeOnSpeed;
    uint8_t led_fadeOffSpeed;
    uint16_t led_holdTime;
    LEDState led_state;
    uint8_t capacitive_sensitivity;
    uint8_t capacitive_debounce;
    volatile uint32_t capacitive_msTouched;
    int capacitive_value;
    bool shouldShow;
    Animation ledControl_animation;
};
#pragma pack(pop)

extern Context context;
#endif