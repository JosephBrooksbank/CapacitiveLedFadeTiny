#include "LedBehaviorController.h"
#include "context.h"

void LedBehaviorController::touchLeds() {
    if (capacitive->isTouched()) {
        led->turnOn();
    } else {
        led->turnOff();
    }
}

void LedBehaviorController::setColorBasedOnCapacitiveValue() {
    // case range not supported, use if else
    if (context.capacitive_value < 4) {
        led->setColor(CRGB::Green);
    } else if (context.capacitive_value < 8) {
        led->setColor(CRGB::Blue);
    } else if (context.capacitive_value < 12) {
        led->setColor(CRGB::Yellow);
    } else if (context.capacitive_value < 26) {
        led->setColor(CRGB::Orange);
    } else {
        led->setColor(CRGB::Red);
    }
}

void LedBehaviorController::changeToNextRainbowTarget() {
    reachedTargetColor = false;
    targetColor = targets[targetIndex];
    startColor = led->getRGB();
    transitionStepCount = 0;
    if (targetIndex == 4) {
        targetIndex = 0;
    } else {
        targetIndex++;
    }
}

void LedBehaviorController::moveTowardsTargetColor() {
    CRGB currentColor = led->getRGB();
    if (currentColor == targetColor) {
        reachedTargetColor = true;
        return;
    }
    CRGB stepColor;
    stepColor.r = map(transitionStepCount, 0, 99, startColor.r, targetColor.r);
    stepColor.g = map(transitionStepCount, 0, 99, startColor.g, targetColor.g);
    stepColor.b = map(transitionStepCount, 0, 99, startColor.b, targetColor.b);
    transitionStepCount++;
    led->setColor(stepColor);
}

void LedBehaviorController::colorSpin() {
    if (reachedTargetColor) {
        changeToNextRainbowTarget();
    }
    moveTowardsTargetColor();
}


void LedBehaviorController::animateColor() {
    if (animate) {
        switch (context.ledControl_animation) {
            case RAINBOW:
                colorSpin();
                break;
            case VALUE_INDICATE:
                setColorBasedOnCapacitiveValue();
                break;
            case NONE:
            default:
                break;
        }
    }
    animate = false;
}

void LedBehaviorController::step() {
        touchLeds();
        if (context.enabledModules.IsAnyOn()) {
            led->turnOn();
        } else {
            led->turnOff();
        }
        animateColor();
        led->step();
}

void LedBehaviorController::tick() {
    animate = true;
}