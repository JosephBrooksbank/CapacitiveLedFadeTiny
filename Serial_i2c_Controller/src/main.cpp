#include <Arduino.h>
#include <Wire.h>
#include "FastLED.h"

#define NUM_LEDS 1

CRGB leds[NUM_LEDS];
uint8_t hue = 0;

void setup() {
    Serial.begin(9600);
    Wire.setClock(100000);
    Wire.begin();
}

uint8_t color[] = {254, 149, 75};
void loop() {
    fill_solid(leds, NUM_LEDS, CHSV(hue, 255, 255));
    hue++;
    if (hue >= 255){
        hue = 0;
    }

    Wire.beginTransmission(0);
    for (int i = 0; i < 3; i++) {
        Wire.write(leds[0].raw[i]);
    }
    Wire.endTransmission();
    Wire.requestFrom(4, 5);
    while (1 < Wire.available()) {
        uint8_t x = Wire.read();
        char c = Wire.read();
        Serial.print(x);
        Serial.print(c);
    }
    uint8_t x = Wire.read();
    Serial.println(x);
    delay(2);
}