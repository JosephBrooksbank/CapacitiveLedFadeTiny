#include <Arduino.h>
#include <config.h>
#include "LED.h"
#include "main.h"
#include "Capacitive.h"
#include "LedBehaviorController.h"
#include "I2CLedControl.hpp"
#include "context.h"
#include "CommandReader.h"
#include "EEPROM.h"

CRGB leds[NUM_LEDS];
LED led(leds, NUM_LEDS);
Capacitive capacitive(CAPACITIVE_PIN);
LedBehaviorController ledBehaviorController(&led, &capacitive);
I2CLedControl i2CLedControl;
CommandReader commandReader(i2CLedControl.getBuffer());


void setup() {
    context.led = &led;
    FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    timerSetup();
    i2CLedControl.setup();
    capacitive.init();
    led.init();
    uint8_t value ;
    EEPROM.get(0, value);

//    EEPROM.put(0, 17);
    if (value == 17) {
        led.flashColor(CRGB::Green);
    } else {
        led.flashColor(CRGB::Red);
    }

}

void loop() {
    if (i2CLedControl.messageReceived) {
        i2CLedControl.messageReceived = false;
        commandReader.parseNewMessage();
    }
    switch (context.mode) {
        case NORMAL:
            ledBehaviorController.step();
            break;
        case RIPPLE:
            break;
        case ON:
            led.turnOn();
            led.step();
            break;
        case CONFIG:
            break;
        case UNKNOWN:
            break;
    }
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
