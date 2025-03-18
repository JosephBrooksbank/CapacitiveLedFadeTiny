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
volatile bool is_update_frame = false;
volatile uint8_t timer_overflows = 0;
const uint8_t goal_overflows = 5;
int referenceCap = 0;
bool lightOn = false;
CRGB leds[NUM_LEDS];
uint16_t cycles_since_last_touched = 0;
Config config;
uint8_t brightness = 0;
uint8_t hue = 100;

const uint16_t timer_count = F_CPU / 2;
void setup_animation_timer() {
    TCB0_CCMP = 65535;
    TCB0_CTRLA = TCB_CLKSEL_CLKDIV2_gc | TCB_ENABLE_bm;
    TCB0_INTCTRL = TCB_CAPT_bm;
    TCB0_CTRLB = TCB_CNTMODE_INT_gc;
}

ISR(TCB0_INT_vect) {
    if (timer_overflows < goal_overflows) {
        timer_overflows++;
    } else {
        timer_overflows = 0;
        is_update_frame = true;
    }
    TCB0_INTFLAGS = TCB_CAPT_bm;
}

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
    hue = config.hue;
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
        case ColorMode::Rainbow:
            return CHSV(hue, 255, 255);
    }
}

void turn_on() {
        lightOn = true;
        brightness = config.max_brightness;
        CRGB color = get_color_for_lights();
        fill_solid(leds, NUM_LEDS, color);
}

void turn_off() {
        lightOn = false;
}

void fade_off() {
    if (brightness > 0) {
        brightness--;
    } 
}

void animate() {
    if (config.color_mode == ColorMode::Rainbow) {
        hue++;
        CRGB color = CHSV(hue, 255, 255);
        fill_solid(leds, NUM_LEDS, color);
    }
    if (!lightOn) {
        fade_off();
    }
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
    setup_animation_timer();
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

    if (is_update_frame) {
        animate();
        is_update_frame = false;
    }
    FastLED[0].showLeds(brightness);
    #ifdef USE_STATUS_LED
        FastLED[1].showLeds(config.max_brightness);
    #endif
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