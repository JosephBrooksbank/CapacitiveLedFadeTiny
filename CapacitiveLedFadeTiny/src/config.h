#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG_LIGHTS true
#define MAX_TIMING_GROUPS 2
#define CAPACITIVE_PIN 3
const int LED_PIN = 1;
#define I2C_ADDRESS 5
#define NUM_GROUPS 4
// if needed in the future - can poll groups rather than individual addresses.
#define GROUP (I2C_ADDRESS % NUM_GROUPS)

#define NUM_MODULES 30


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