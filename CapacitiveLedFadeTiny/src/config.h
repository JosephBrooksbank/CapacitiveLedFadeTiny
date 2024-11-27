#ifndef CONFIG_H
#define CONFIG_H

// How sensitive the capacitive sensor is. Lower = more sensitive.
#define CAPACITIVE_SENSITIVITY 5
const int ledStripPin = 1;
#define I2C_ADDRESS 5

#define BUFFER_SIZE 10

// physical layout of the table, approximated
#define NUM_ROWS 2
#define NUM_COLS 2
#include "Arduino.h"
const uint8_t ADDRESS_MAP[NUM_ROWS][NUM_COLS] = {
        {5,2},
        {3, 4}
};

// How many LEDs in the string. Limited by memory available.
// Max with serial: 30
// Max without: 52+
#define NUM_LEDS 30
#define COLOR_ORDER GRB

#endif