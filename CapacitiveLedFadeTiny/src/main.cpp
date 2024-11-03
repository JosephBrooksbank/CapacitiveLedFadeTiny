#include <Arduino.h>
#include <ADCTouch.h>
#include <config.h>
#include "FastLED.h"
#include "Serial.h"
#include "I2CLedControl.h"
#include "I2CEchoHandler.h"


const int ledPin = 0;
//const int ledStripPin = 1;
const int capPin = 3;
CRGB leds[NUM_LEDS];
bool touched;
I2CLedControl i2cHandler(touched);
//I2CEchoHandler i2cHandler;


const uint8_t ledDimSpeed = 8;
int capacitiveReference;
uint8_t brightness = 0;
uint8_t ledDimDelay = 10;
uint8_t lastDetected = 0;

void ledSetup();
void dimLeds();
void setAllLedColor(const CRGB& color);
void setup() {
    SERIAL_BEGIN(9600);
    ledSetup();
    i2cHandler.setup();
//    FastLED.addLeds<WS2812B, ledStripPin, COLOR_ORDER>(leds, NUM_LEDS);
    pinMode(ledPin, OUTPUT);
    capacitiveReference = ADCTouch.read(capPin, 500);
}


void loop() {
    int capacitiveValue = ADCTouch.read(capPin, 100);
    capacitiveValue -= capacitiveReference;

//    SERIAL_PRINTLN(capacitiveValue);
    if (capacitiveValue > CAPACITIVE_SENSITIVITY) {
        SERIAL_PRINTLN("touched");
        brightness = 255;
        touched = true;
        lastDetected = 0;
    } else if (brightness > 0 && lastDetected > ledDimDelay) {
        touched = false;
        dimLeds();
    } else {
        lastDetected++;
    }
    FastLED.setBrightness(brightness);
    FastLED.show();
    delay(1);
}

void ledSetup() {
    FastLED.addLeds<WS2812B, ledStripPin, COLOR_ORDER>(leds, NUM_LEDS);
    setAllLedColor(CRGB::White);
    FastLED.setBrightness(0);


}

void setAllLedColor(const CRGB& color) {
    fill_solid(leds, NUM_LEDS, color);
}

void dimLeds() {
    if (brightness < ledDimSpeed) {
        brightness = 0;
    }
    else {
        brightness -= ledDimSpeed;
    }
}