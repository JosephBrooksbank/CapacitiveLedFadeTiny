#include <Arduino.h>
#include "Wire.h"
#include "config.h"

Config testConfig = {
    .version = 1,
    .touch_sense = DEFAULT_TOUCH_SENSE,
    .off_delay = DEFAULT_OFF_DELAY,
    .max_brightness = 100,
    .color_mode = ColorMode::Random,
    .single_color_color = CRGB::Blue
};

void set_scl_min() {
    // the absolute slowest that SCL can run according to the hardware,
    // about 500hz
    TWSR |= 1 << TWPS0;
    TWSR |= 1 << TWPS1;
    TWBR = 0b11111111;
}

void set_scl_max() {
    TWSR &= ~(1 << TWPS0);
    TWSR &= ~(1 << TWPS1);
    TWBR = 12;

}

void setup() {
    Wire.begin();
    set_scl_max();

    Wire.beginTransmission(0);
    Wire.write((byte*)&testConfig, sizeof(Config));
    Wire.endTransmission();
}



void loop() {

    // Wire.beginTransmission(0);
    // Wire.write('a');
    // Wire.endTransmission();
    delay(2000);
}