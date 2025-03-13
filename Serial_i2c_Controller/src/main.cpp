#include <Arduino.h>
#include "Wire.h"

void setup() {
    Wire.begin();

}

void loop() {

    Wire.beginTransmission(0);
    Wire.write('a');
    Wire.write('b');
    Wire.endTransmission();
    delay(2000);
}