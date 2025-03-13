#include "FastLED.h"
#include "adcTouch.h"
#include "Wire.h"
#include "status_led.h"
#include "config.h"
#include "millis_fix.h"



void receiveDataWire(int numBytes);
volatile byte i2c_buffer[30];
volatile uint16_t i2c_length = 0;
int referenceCap = 0;
bool lightOn = false;
CRGB leds[NUM_LEDS];
uint16_t cycles_since_last_touched = 0;

void setup_i2c() {
    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(receiveDataWire);

}

void setup_leds() {
    FastLED.addLeds<LED_DRIVER, LED_PIN, GRB>(leds, NUM_LEDS);
}

void setup() {
    Serial.begin(BAUD_RATE);
    setup_i2c();
    setup_leds();
    setup_status_led();
    FastLED.setBrightness(50);
    #ifdef USE_ONBOARD_LED
        pinMode(ENABLE_DEBUG_LED_PIN, OUTPUT); 
        digitalWrite(ENABLE_DEBUG_LED_PIN, true);
    #endif

    referenceCap = read(SENSE_PIN, 64);
    set_status_led(CRGB::Red);
}


int reading = 0;
void loop() {
    Serial.println("looping");
    reading = read(SENSE_PIN, 64) - referenceCap;

    if (reading >= TOUCH_SENSE) {
        cycles_since_last_touched = 0;
    } else {
        if (cycles_since_last_touched < OFF_DELAY) {
            cycles_since_last_touched++;
        }
    }

    if (cycles_since_last_touched < OFF_DELAY && !lightOn) {
        lightOn = true;
        CRGB randomColor = CHSV(random(192), 255, 255);
        fill_solid(leds, NUM_LEDS, randomColor);
        FastLED.show();
    } else if (cycles_since_last_touched >= OFF_DELAY && lightOn) {
        lightOn = false;
        leds[0] = CRGB::Black;
        FastLED.show();
    }
}


void receiveDataWire(int numBytes) {
    for (uint8_t i = 0; i < numBytes; i++) {
        i2c_buffer[i] = Wire.read();
    }
    i2c_length = numBytes;
}