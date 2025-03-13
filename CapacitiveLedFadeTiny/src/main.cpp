#include "FastLED.h"
#include "adcTouch.h"
#include "Wire.h"
#include "status_led.h"
#include "config.h"
#include "millis_fix.h"
#include "SoftwareSerial.h"



void receiveDataWire(int numBytes);
volatile byte input_buffer[30];
volatile uint8_t input_length = 0;
volatile bool new_message = false;
int referenceCap = 0;
bool lightOn = false;
CRGB leds[NUM_LEDS];
uint16_t cycles_since_last_touched = OFF_DELAY;

void setup_i2c() {
    Wire.begin(I2C_ADDRESS, true);
    Wire.onReceive(receiveDataWire);
    Serial.println("Using i2c");

}

void setup_leds() {
    FastLED.addLeds<LED_DRIVER, LED_PIN, GRB>(leds, NUM_LEDS);
}

#ifdef USE_SOFTWARE_SERIAL
    SoftwareSerial soft_serial = SoftwareSerial(RX_PIN, TX_PIN);
#endif

void setup() {
    Serial.begin(BAUD_RATE);
    #ifdef USE_I2C
        setup_i2c();
    #endif
    #ifdef USE_SOFTWARE_SERIAL
        pinMode(RX_PIN, INPUT);
        pinMode(TX_PIN, OUTPUT);
        soft_serial.begin(SOFTWARE_BAUD);
        Serial.println("Using software serial");

    #endif
    setup_leds();
    setup_status_led();
    FastLED.setBrightness(50);
    #ifdef USE_ONBOARD_LED
        pinMode(ENABLE_DEBUG_LED_PIN, OUTPUT); 
        digitalWriteFast(ENABLE_DEBUG_LED_PIN, true);
    #endif

    referenceCap = read(SENSE_PIN, 64);
    set_status_led(CRGB::Red);
}


int reading = 0;
void loop() {

    #ifdef USE_SOFTWARE_SERIAL
    if (soft_serial.available()) {
        new_message = true;
        input_length = 0;
        while (soft_serial.available()) {
            input_buffer[input_length++] = soft_serial.read();
        }
    }
    #endif

    if (new_message) {
        new_message = false;
        Serial.printf("Received message of length %d\n", input_length);
        Serial.print("Message: ");
        for (uint8_t i = 0; i < input_length; i++) {
            Serial.print((char)input_buffer[i]);
        }
        Serial.println();
    }


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
        input_buffer[i] = Wire.read();
    }
    input_length = numBytes;
    new_message = true;
}