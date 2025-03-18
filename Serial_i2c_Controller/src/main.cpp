#include <Arduino.h>
#include "Wire.h"
#include "config.h"

Config testConfig = {
    .should_store = false,
    .touch_sense = DEFAULT_TOUCH_SENSE,
    .off_delay = DEFAULT_OFF_DELAY,
    .max_brightness = 100,
    .color_mode = ColorMode::Rainbow,
    .single_color_color = CRGB::Blue,
    .fade_amount = DEFAULT_FADE_AMOUNT,
    .hue = DEFAULT_HUE
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
    set_scl_min();

    Wire.beginTransmission(0);
    Wire.write((byte*)&testConfig, sizeof(Config));
    Wire.endTransmission();
}


void rainbow_sync() {
    // this ratio was accidental but ended up being essentially flawless for keeping in sync even with
    // i2c messages getting in the way. Keeping ite, even if the original math was flawed
    delay(600);
    testConfig.hue+=100;
    Wire.beginTransmission(0);
    Wire.write((byte*)&testConfig, sizeof(Config));
    Wire.endTransmission();
}


void loop() {
    rainbow_sync();
}