#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

#define TOUCH_SENSE 5
#define OFF_DELAY 200

#define BAUD_RATE 115200
#define NUM_LEDS 30
#define ENABLE_DEBUG_LED_PIN PIN_PA7
#define STATUS_PIN PIN_PA4
#define SENSE_PIN PIN_PA6
#define LED_DRIVER WS2812
#define LED_PIN PIN_PA5


// i2c stuff
#define USE_I2C
#define I2C_ADDRESS 14


// Software Serial Stuff
#ifndef USE_I2C
    #define USE_SOFTWARE_SERIAL
    #define SOFTWARE_BAUD 9600
    #define RX_PIN PIN_PB0
    #define TX_PIN PIN_PB1
#endif


#define USE_ONBOARD_LED
#define USE_STATUS_LED


#endif