#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"


#define CONFIG_EEPROM_ADDRESS 0
struct Config {
    uint8_t version;
    uint8_t touch_sense;
    uint8_t off_delay;
    uint8_t max_brightness;
}; 

///// RUNTIME CONFIG (set in EEPROM)
#define CONFIG_VERSION 1
#define DEFAULT_TOUCH_SENSE 5
#define DEFAULT_OFF_DELAY 200
#define DEFAULT_MAX_BRIGHTNESS 10

Config get_config();
void set_config(Config &conf);
Config setup_config();


///// CONST CONFIG (no change at runtime)
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