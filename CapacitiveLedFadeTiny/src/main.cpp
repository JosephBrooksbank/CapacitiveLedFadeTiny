#include "FastLED.h"
#include "adcTouch.h"
#include "Wire.h"
#include "status_led.h"
#include "config.h"
#include "millis_fix.h"
#include "SoftwareSerial.h"



void receiveDataWire(int numBytes);
void receiveDataSoftSerial();
volatile byte input_buffer[sizeof(Config)+1];
volatile uint8_t input_length = 0;
volatile bool new_message = false;
int referenceCap = 0;
bool lightOn = false;
CRGB leds[NUM_LEDS];
uint16_t cycles_since_last_touched = 0;
Config config;

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

void applyConfig() {
    FastLED.setBrightness(config.max_brightness);
}

void handleNewMessage() {
        new_message = false;
        Serial.printf("Received message of length %d\n", input_length);

        memcpy(&config, (const void*)input_buffer, sizeof(Config));
        Serial.printf("Received new config data, version %d, max_brightness %d", config.version, config.max_brightness);
        set_config(config);
        applyConfig();
}

CRGB get_color_for_lights() {
    switch (config.color_mode) {
        case ColorMode::Random: 
            return CHSV(random(192), 255, 255);
        case ColorMode::Single:
            return config.single_color_color;
    }
}

void turn_on() {
        lightOn = true;
        CRGB color = get_color_for_lights();
        fill_solid(leds, NUM_LEDS, color);
}

void turn_off() {
        lightOn = false;
        fill_solid(leds, NUM_LEDS, CRGB::Black);
}

int reading = 0;
void detect_touch() {
    reading = read(SENSE_PIN, 64) - referenceCap;

    if (reading >= config.touch_sense) {
        cycles_since_last_touched = 0;
    } else {
        if (cycles_since_last_touched < config.off_delay) {
            cycles_since_last_touched++;
        }
    }
}

bool should_turn_on() {
    return cycles_since_last_touched < config.off_delay && !lightOn;
}

bool should_turn_off() {
    return cycles_since_last_touched >= config.off_delay && lightOn;
}

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
    #ifdef USE_ONBOARD_LED
        pinMode(ENABLE_DEBUG_LED_PIN, OUTPUT); 
        digitalWriteFast(ENABLE_DEBUG_LED_PIN, true);
    #endif
    config = setup_config();
    FastLED.setBrightness(config.max_brightness);
    cycles_since_last_touched = config.off_delay;

    referenceCap = read(SENSE_PIN, 64);
    set_status_led(CRGB::Green);
}


void loop() {

    receiveDataSoftSerial();
    if (new_message) {
        handleNewMessage();
    }

    detect_touch();


    if (should_turn_on()) {
        turn_on();
    } else if (should_turn_off()) {
        turn_off();
    }
    FastLED.show();
}


void receiveDataWire(int numBytes) {
    if (numBytes != sizeof(Config)) {
        set_status_led(CRGB::Red);
        return;
    } else {
        set_status_led(CRGB::Blue);
    }
    for (uint8_t i = 0; i < numBytes; i++) {
        input_buffer[i] = Wire.read();
    }
    input_length = numBytes;
    new_message = true;
}

void receiveDataSoftSerial() {
    #ifdef USE_SOFTWARE_SERIAL
    if (soft_serial.available()) {
        new_message = true;
        input_length = 0;
        while (soft_serial.available()) {
            input_buffer[input_length++] = soft_serial.read();
        }
    }
    #endif
}