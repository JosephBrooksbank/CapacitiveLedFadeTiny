#include <Arduino.h>
#include <ADCTouch.h>
#include <config.h>
#include "FastLED.h"
#include "Serial.h"
#include "I2CLedControl.hpp"
#include "I2CEchoHandler.hpp"

/// commands
// r: 'read' -> function as normal. turn on led in response to touch and report status when requested
// o: 'on' -> turn on led, continue to report status when requested. Will continue to be on until given a different command.
// c: 'color' -> set current led strip color, without worrying about current brightness. Goes back to previous command after. Uses 3 bytes from buffer as RGB
// f: 'fade' -> change turn on mode to 'fade in', rather than turning on instantly. next byte is how fast to turn on
// i: 'instant' -> default turn on mode, when touch (or 'on') then set brightness to max with no ramp up

const int ledPin = 0;
//const int ledStripPin = 1;
const int capPin = 3;
CRGB leds[NUM_LEDS];
bool touched;
I2CLedControl i2cHandler(touched);
//I2CEchoHandler i2cHandler;


const uint8_t ledDimSpeed = 8;
uint8_t ledFadeOnSpeed = 8;
int capacitiveReference;
uint8_t brightness = 0;
uint8_t ledDimDelay = 10;
uint8_t lastDetected = 0;
char currentCommand = 'r';
char previousCommand = 'r';
char turnOnMode = 'i';

void turnOn();
void fadeOn();
void fadeOff();
void rBehavior();
void oBehavior();
void cBehavior();
void fBehavior();
void iBehavior();

void ledSetup();
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
        previousCommand = currentCommand;
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
        case 'c': {
            cBehavior();
            break;
        }
        case 'f': {
            fBehavior();
            break;
        }
        case 'i': {
            iBehavior();
            break;
        }
        default:
            break;
    }
    delay(1);
}

void rBehavior() {
    if (touched) {
        turnOn();
        lastDetected = 0;
    } else if (brightness > 0 && lastDetected > ledDimDelay) {
        fadeOff();
    } else {
        lastDetected++;
    }

    FastLED.setBrightness(brightness);
    FastLED.show();
}

void oBehavior() {
    turnOn();
    lastDetected = 0;
    FastLED.setBrightness(brightness);
    FastLED.show();
}

void cBehavior() {
    byte* buffer = i2cHandler.getBuffer();
    uint8_t red = buffer[0];
    uint8_t green = buffer[1];
    uint8_t blue = buffer[2];
    setAllLedColor(CRGB(red, green, blue));
    currentCommand = previousCommand;
}

void fBehavior() {
    byte* buffer = i2cHandler.getBuffer();
    ledFadeOnSpeed = buffer[0];
    turnOnMode = 'f';
    currentCommand = previousCommand;
}

void iBehavior() {
    turnOnMode = 'i';
    currentCommand = previousCommand;
}

void ledSetup() {
    FastLED.addLeds<WS2812B, ledStripPin, COLOR_ORDER>(leds, NUM_LEDS);
    setAllLedColor(CRGB::White);
    FastLED.setBrightness(0);


}

void setAllLedColor(const CRGB& color) {
    fill_solid(leds, NUM_LEDS, color);
}

void turnOn() {
    switch (turnOnMode) {
        default:
        case 'o': {
            brightness = 255;
            break;
        }
        case 'f': {
            fadeOn();
        }
    }
}
void fadeOn() {
    if (brightness + ledFadeOnSpeed > 255) {
        brightness = 255;
    } else {
        brightness += ledFadeOnSpeed;
    }
}

void fadeOff() {
    if (brightness < ledDimSpeed) {
        brightness = 0;
    }
    else {
        brightness -= ledDimSpeed;
    }
}