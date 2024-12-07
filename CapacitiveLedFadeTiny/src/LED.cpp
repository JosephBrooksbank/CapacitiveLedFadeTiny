#include "LED.h"
#include "context.h"

void LED::init() {
    FastLED.clear();
    FastLED.setBrightness(50);
    setColor(CRGB::White);
    FastLED.show();
}

void LED::setColor(const CRGB &color) {
    fill_solid(leds, numLeds, color);
}

void LED::turnOn() {
    uint8_t brightness = FastLED.getBrightness();
    if (brightness == 255) {
        context.led_state = HOLD;
        return;
    }
    holdTime = context.led_holdTime;
    context.led_state = FADE_ON;
}

void LED::turnOff() {
    uint8_t brightness = FastLED.getBrightness();
    if (brightness == 0) {
        context.led_state = OFF;
        return;
    }
    context.led_state = FADE_OFF;
}

void LED::updateBrightness() {
    uint8_t brightness = FastLED.getBrightness();
    switch (context.led_state) {
        case FADE_ON: {
            if (brightness + context.led_fadeOnSpeed > 255) {
                brightness = 255;
            } else {
                brightness = brightness + context.led_fadeOnSpeed;
            }
            if (brightness == 255) {
                context.led_state = HOLD;
            }
            FastLED.setBrightness(brightness);
            break;
        }
        case FADE_OFF: {
            // hold time is decremented by the tick function
            if (holdTime > 0) {
                break;
            }
            if (brightness - context.led_fadeOffSpeed < 0) {
                brightness = 0;
            } else {
                brightness = brightness - context.led_fadeOffSpeed;
            }
            if (brightness == 0) {
                context.led_state = OFF;
            }
            FastLED.setBrightness(brightness);
            break;
        }
        case HOLD: {
            break;
        }
        case OFF: {
            break;
        }
    }
}

void LED::step() {
        updateBrightness();
        FastLED.show();
}

void LED::tick() {
    if (context.led_state == FADE_OFF && holdTime > 0) {
        if (holdTime > 0) {
            holdTime--;
        }
    }
}