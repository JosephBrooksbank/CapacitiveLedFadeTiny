#ifndef LED_BEHAVIOR_CONTROLLER_H
#define LED_BEHAVIOR_CONTROLLER_H
#include "Arduino.h"
#include "LED.h"
#include "Capacitive.h"

class LedBehaviorController {
public:
    LedBehaviorController(LED* led, Capacitive* capacitive) : led(led), capacitive(capacitive) {};
    void step();
    void tick();

private:
    void animateColor();
    void touchLeds();
    void colorSpin();
    void setColorBasedOnCapacitiveValue();
    void changeToNextRainbowTarget();
    void moveTowardsTargetColor();
    CRGB targetColor = CRGB::White;
    uint8_t transitionStepCount = 0;
    CRGB startColor = CRGB::White;
    bool reachedTargetColor = true;
    volatile bool animate = true;
    LED* led;
    Capacitive* capacitive;
    CRGB targets[5] = {
            CRGB::Red,
            CRGB::Green,
            CRGB::Blue,
            CRGB::Yellow,
            CRGB::Orange
    };
    int targetIndex = 0;
};

#endif
