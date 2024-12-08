#include <Arduino.h>
#include <config.h>
#include "LED.h"
#include "main.h"
#include "Capacitive.h"
#include "LedBehaviorController.h"
#include "I2CLedControl.hpp"

CRGB leds[NUM_LEDS];
LED led(leds, NUM_LEDS);
Capacitive capacitive(CAPACITIVE_PIN);
LedBehaviorController ledBehaviorController(&led, &capacitive);
I2CLedControl i2CLedControl;


void setup() {
    FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    timerSetup();
    i2CLedControl.setup();
    capacitive.init();
    led.init();
}

void loop() {
    ledBehaviorController.step();
    delay(1);
}


void timerSetup() {
    TCCR1 = 0; // reset timer1 config
    GTCCR |= (1 << PSR1); // reset prescaler
    TCCR1 = (1 << CTC1) | (1 << CS12) | (1 << CS11); // CTC mode, prescaler = 64
    OCR1C = 124; // Compare value for 1ms
    TIMSK |= (1 << OCIE1A); // enable interrupt

    sei(); // enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
    capacitive.tick();
    ledBehaviorController.tick();
    led.tick();
}
