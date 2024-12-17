//
// Created by joebr on 12/16/2024.
//

#include "ConfigValues.h"
#include "EEPROM.h"

uint8_t ConfigValues::getCurrentVersion() {
    uint8_t v;
    EEPROM.get(0, v);
    return v;
}

void ConfigValues::setCurrentVersion(uint8_t v) {
    EEPROM.put(0, v);
}

Mode ConfigValues::mode() {
    Mode m;
    EEPROM.get(1, m);
    return m;
}

void ConfigValues::setMode(Mode value) {
    EEPROM.put(1, value);
}

uint8_t ConfigValues::led_fadeOffSpeed() {
    uint8_t v;
    EEPROM.get(2, v);
    return v;
}

void ConfigValues::setLed_fadeOffSpeed(uint8_t value) {
    EEPROM.put(2, value);
}

uint8_t ConfigValues::led_fadeOnSpeed() {
    uint8_t v;
    EEPROM.get(3, v);
    return v;
}

void ConfigValues::setLed_fadeOnSpeed(uint8_t value) {
    EEPROM.put(3, value);
}

uint16_t ConfigValues::led_holdTime() {
    uint16_t v;
    EEPROM.get(4, v);
    return v;
}

void ConfigValues::setLed_holdTime(uint16_t value) {
    EEPROM.put(4, value);
}

LEDState ConfigValues::led_state() {
    LEDState m;
    EEPROM.get(6, m);
    return m;
}

void ConfigValues::setLed_state(LEDState value) {
    EEPROM.put(6, value);
}

uint8_t ConfigValues::capacitive_sensitivity() {
    uint8_t v;
    EEPROM.get(7, v);
    return v;
}
void ConfigValues::setCapacitive_sensitivity(uint8_t value) {
    EEPROM.put(7, value);
}

uint8_t ConfigValues::capacitive_debounce() {
    uint8_t v;
    EEPROM.get(8, v);
    return v;
}

void ConfigValues::setCapacitive_debounce(uint8_t value) {
    EEPROM.put(8, value);
}

Animation ConfigValues::ledControl_animation() {
    Animation m;
    EEPROM.get(9, m);
    return m;
}

void ConfigValues::setLedControl_animation(Animation value) {
    EEPROM.put(9, value);
}

uint8_t ConfigValues::timingGroupCount() {
    uint8_t v;
    EEPROM.get(10, v);
    return v;
}

void ConfigValues::setTimingGroupCount(uint8_t value) {
    EEPROM.put(10, value);
}


void ConfigValues::ensureDefaultValuesInEEProm() {
    uint8_t version = getCurrentVersion();
    if (version == currentVersion) {
        return;
    }
    setMode(DEFAULT_MODE);
    setLed_fadeOffSpeed(DEFAULT_LED_FADE_OFF_SPEED);
    setLed_fadeOnSpeed(DEFAULT_LED_FADE_ON_SPEED);
    setLed_holdTime(DEFAULT_LED_HOLD_TIME);
    setLed_state(DEFAULT_LED_STATE);
    setCapacitive_sensitivity(DEFAULT_CAPACITIVE_SENSITIVITY);
    setCapacitive_debounce(DEFAULT_CAPACITIVE_DEBOUNCE);
    setLedControl_animation(DEFAULT_LED_CONTROL_ANIMATION);
    setTimingGroupCount(DEFAULT_TIMING_GROUP_COUNT);
    setCurrentVersion(version);
}

TimingGroup ConfigValues::getTimingGroup(uint8_t index) {
    TimingGroup timingGroup;
    uint8_t cursor = (TIMING_GROUP_START_ADDRESS + index * sizeof (TimingGroup) );
    EEPROM.get(cursor, timingGroup);
    return timingGroup;
}
void ConfigValues::setTimingGroup(uint8_t index, TimingGroup timingGroup) {
    uint8_t cursor = (TIMING_GROUP_START_ADDRESS + index * sizeof (TimingGroup) );
    EEPROM.put(cursor, timingGroup);
}

