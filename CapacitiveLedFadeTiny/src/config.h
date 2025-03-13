#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

#define TOUCH_SENSE 5
#define OFF_DELAY 200

#define BAUD_RATE 115200
#define I2C_ADDRESS 14
#define NUM_LEDS 30
#define ENABLE_DEBUG_LED_PIN PIN_A7
#define STATUS_PIN PIN_A4
#define SENSE_PIN PIN_A6
#define LED_DRIVER WS2812
#define LED_PIN PIN_A5

#define USE_ONBOARD_LED
#define USE_STATUS_LED


#endif