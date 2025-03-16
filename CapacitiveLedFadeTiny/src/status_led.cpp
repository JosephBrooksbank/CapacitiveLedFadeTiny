#include "Arduino.h"
#include "status_led.h"
#include "config.h"


CRGB status_leds[1];
#define STATUS_LED status_leds[0]

void setup_status_led() {
    #ifdef USE_STATUS_LED
        FastLED.addLeds<LED_DRIVER, STATUS_PIN, GRB>(status_leds, 1);
    #endif
}

void set_status_led(const CRGB color) {
    #ifdef USE_STATUS_LED
        STATUS_LED = color;
    #endif

}