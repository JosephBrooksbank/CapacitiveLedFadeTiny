#ifndef CONTEXT_H
#define CONTEXT_H
#include "Arduino.h"
#include "config.h"
#include "AddressMap.h"
#include "TimingGroup.h"
#include "LED.h"

enum class Mode : uint8_t {
    NORMAL,
    RIPPLE,
    ON,
    CONFIG,
    UNKNOWN,
};

enum class LEDState : uint8_t {
    OFF,
    FADE_ON,
    HOLD,
    FADE_OFF
};

enum class Animation : uint8_t {
    NONE,
    VALUE_INDICATE,
    RAINBOW
};

#pragma pack(push, 1) // Force 1-byte alignment so we can send the struct over I2C
struct Context {
    volatile uint32_t capacitive_msTouched;
    int capacitive_value;
    AddressMap enabledModules;
    LED* led;
};
#pragma pack(pop)

extern Context context;
#endif