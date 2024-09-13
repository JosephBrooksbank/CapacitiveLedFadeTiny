#include <Arduino.h>
#include <ADCTouch.h>
#include <config.h>
#include <FastLED.h>


#ifdef USE_SERIAL

#include <SendOnlySoftwareSerial.h>

const int txPin = 4;
SendOnlySoftwareSerial mySerial(txPin);
#define SERIAL_BEGIN(baud) mySerial.begin(baud)
#define SERIAL_PRINTLN(x) mySerial.println(x)

#else
#define SERIAL_BEGIN(baud) // Empty
#define SERIAL_PRINTLN(x)  // Empty
#endif

const int ledPin = 0;
const int ledStripPin = 1;
const int capPin = 3;
CRGB leds[NUM_LEDS];


const uint8_t ledDimSpeed = 8;
int capacitiveReference;
uint8_t brightness = 0;

void setup() {
    SERIAL_BEGIN(9600);
    FastLED.addLeds<WS2812B, ledStripPin, COLOR_ORDER>(leds, NUM_LEDS);
    pinMode(ledPin, OUTPUT);
    capacitiveReference = ADCTouch.read(capPin, 500);
    for (int i; i < NUM_LEDS; i++) {
        leds[i] = CRGB::White;
    }
    FastLED.setBrightness(0);
}

void dimLeds() {
    if (brightness - ledDimSpeed < 0) {
        brightness = 0;
    } else {
        brightness -= ledDimSpeed;
    }
}

void loop() {
    int capacitiveValue = ADCTouch.read(capPin, 100);
    capacitiveValue -= capacitiveReference;

    SERIAL_PRINTLN(capacitiveValue);
    if (capacitiveValue > CAPACITIVE_SENSITIVITY) {
        brightness = 255;
    }
    FastLED.setBrightness(brightness);
    FastLED.show();
    dimLeds();
    delay(1);
}