//
// Created by joebr on 12/16/2024.
//

#ifndef CAPACITIVELEDFADETINY_CONFIGVALUES_H
#define CAPACITIVELEDFADETINY_CONFIGVALUES_H

#include "context.h"

/// defaults
#define DEFAULT_MODE Mode::NORMAL
#define DEFAULT_LED_FADE_ON_SPEED 2
#define DEFAULT_LED_FADE_OFF_SPEED 2
#define DEFAULT_LED_HOLD_TIME 400
#define DEFAULT_LED_STATE LEDState::OFF
#define DEFAULT_CAPACITIVE_SENSITIVITY 10
#define DEFAULT_CAPACITIVE_DEBOUNCE 4
#define DEFAULT_LED_CONTROL_ANIMATION Animation::RAINBOW
#define DEFAULT_TIMING_GROUP_COUNT 0

#define TIMING_GROUP_START_ADDRESS 50


class ConfigValues {
public:
    static const uint8_t currentVersion = 1;
    static uint8_t getCurrentVersion();
    static void setCurrentVersion(uint8_t value);
    static Mode mode();
    static void setMode(Mode value);
    static uint8_t led_fadeOffSpeed();
    static void setLed_fadeOffSpeed(uint8_t value);
    static uint8_t led_fadeOnSpeed();
    static void setLed_fadeOnSpeed(uint8_t value);
    static uint16_t led_holdTime();
    static void setLed_holdTime(uint16_t value);
    static LEDState led_state();
    static void setLed_state(LEDState value);
    static uint8_t capacitive_sensitivity();
    static void setCapacitive_sensitivity(uint8_t value);
    static uint8_t capacitive_debounce();
    static void setCapacitive_debounce(uint8_t value);
    static Animation ledControl_animation();
    static void setLedControl_animation(Animation value);
    static uint8_t timingGroupCount();
    static void setTimingGroupCount(uint8_t value);
    static void ensureDefaultValuesInEEProm();
    static TimingGroup getTimingGroup(uint8_t index);
    static void setTimingGroup(uint8_t index, TimingGroup timingGroup);
};


#endif //CAPACITIVELEDFADETINY_CONFIGVALUES_H
