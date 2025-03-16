#include <Arduino.h>
#include "Wire.h"
#include "config.h"

Config testConfig = {
    .version = 1,
    .touch_sense = DEFAULT_TOUCH_SENSE,
    .off_delay = DEFAULT_OFF_DELAY,
    .max_brightness = 50
};

void setup() {
    Wire.begin();
    Wire.setClock(500);

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