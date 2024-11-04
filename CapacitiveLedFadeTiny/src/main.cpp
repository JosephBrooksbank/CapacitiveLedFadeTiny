#include <Arduino.h>
#include <ADCTouch.h>
#include <config.h>
#include "FastLED.h"
#include "Serial.h"
#include "I2CLedControl.h"
#include "I2CEchoHandler.h"

/// commands
// r: 'read' -> function as normal. turn on led in response to touch and report status when requested
// o: 'on' -> turn on led, continue to report status when requested. Will continue to be on until given a different command.

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
char currentCommand = 'r';

void rBehavior();
void oBehavior();
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
    if (i2cHandler.isNewCommand) {
        currentCommand = i2cHandler.command;
    }

    int capacitiveValue = ADCTouch.read(capPin, 100);
    capacitiveValue -= capacitiveReference;
//    SERIAL_PRINTLN(capacitiveValue);
    if (capacitiveValue > CAPACITIVE_SENSITIVITY) {
        SERIAL_PRINTLN("touched");
        touched = true;
    } else {
        touched = false;
    }
    switch (currentCommand) {
        case 'r': {
            rBehavior();
            break;
        }
        case 'o': {
            oBehavior();
            break;
        }
        default:
            break;
    }
    delay(1);
}

void rBehavior() {
    if (touched) {
        brightness = 255;
        lastDetected = 0;
    } else if (brightness > 0 && lastDetected > ledDimDelay) {
        dimLeds();
    } else {
        lastDetected++;
    }

    FastLED.setBrightness(brightness);
    FastLED.show();
}

void oBehavior() {
    brightness = 255;
    lastDetected = 0;
    FastLED.setBrightness(brightness);
    FastLED.show();
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