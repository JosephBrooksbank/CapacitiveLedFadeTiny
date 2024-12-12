#ifndef LED_H
#define LED_H
#include "FastLED.h"


class LED {
public:
    LED(CRGB *leds, uint8_t numLeds) : leds(leds), numLeds(numLeds) {};
    // set initial state of the leds
    void init();
    // update leds 1 frame
    void step();
    // called by isr every ms
    void tick();
    void turnOn();
    void turnOff();
    void flashColor(const CRGB &color);
    // set all leds to a color
    void setColor(const CRGB &color);
    CRGB getRGB();

private:
    void updateBrightness();
    CRGB *leds;
    volatile uint16_t holdTime = 0;
    uint8_t numLeds;
};

#endif