#include <Arduino.h>
#include <config.h>
#include "LED.h"
#include "main.h"
#include "Capacitive.h"

CRGB leds[NUM_LEDS];
LED led(leds, NUM_LEDS);
Capacitive capacitive(CAPACITIVE_PIN);


void setup() {
    FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    timerSetup();
    capacitive.init();
    led.init();
}

void loop() {
    if (capacitive.isTouched()){
        led.turnOn();
    } else {
        led.turnOff();
    }

    led.step();
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
    led.tick();
}
