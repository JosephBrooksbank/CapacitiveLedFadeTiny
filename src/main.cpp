#include <Arduino.h>
#include <ADCTouch.h>
#include <config.h>
#include <FastLED.h>
#include "Wire.h"


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


void requestEvent();
void receiveEvent(int howMany);
void setup() {
    Wire.begin(I2C_ADDRESS);
    Wire.onRequest(requestEvent);
    Wire.onReceive(receiveEvent);
    SERIAL_BEGIN(9600);
    FastLED.addLeds<WS2812B, ledStripPin, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setCorrection(TypicalLEDStrip);
    pinMode(ledPin, OUTPUT);
    capacitiveReference = ADCTouch.read(capPin, 500);
    fill_solid(leds, NUM_LEDS, CRGB::White);
    FastLED.setBrightness(0);
}

void dimLeds() {
    if (brightness - ledDimSpeed < 0) {
        brightness = 0;
    } else {
        brightness -= ledDimSpeed;
    }
}

char receivedColor[3];
char currentColor[3];
void loop() {
    if (memcmp(receivedColor, currentColor, 3) != 0) {
        fill_solid(leds, NUM_LEDS, CRGB(receivedColor[0], receivedColor[1], receivedColor[2]));
        memcpy(currentColor, receivedColor, 3);
    }
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


void receiveEvent(int howMany){

    for (int i =0; i < howMany; i++) {
        receivedColor[i] = Wire.read();
    }
}

void requestEvent() {
    //5 bytes
    Wire.write(currentColor[0]);
    Wire.write(",");
    Wire.write(currentColor[1]);
    Wire.write(",");
    Wire.write(currentColor[2]);
}
