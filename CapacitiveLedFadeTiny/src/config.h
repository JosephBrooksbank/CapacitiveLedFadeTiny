#ifndef CONFIG_H
#define CONFIG_H

// How sensitive the capacitive sensor is. Lower = more sensitive.
#define CAPACITIVE_SENSITIVITY 5
const int ledStripPin = 1;
const uint8_t I2C_ADDRESS = 3;
const uint8_t txPin = 4;

// How many LEDs in the string. Limited by memory available.
// Max with serial: 30
// Max without: 52+
#define NUM_LEDS 30
#define COLOR_ORDER GRB

// if we should include (software) serial for logging
#define USE_SERIAL


#endif