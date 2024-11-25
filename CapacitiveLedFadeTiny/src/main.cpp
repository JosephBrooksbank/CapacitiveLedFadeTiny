#include <Arduino.h>
#include <ADCTouch.h>
#include <config.h>
#include "FastLED.h"
#include "Serial.h"
#include "I2CLedControl.hpp"
#include "I2CEchoHandler.hpp"

/// commands
// r: 'read' -> function as normal. turn on led in response to touch and report status when requested
// o: 'on' -> turn on led, continue to report status when requested. Will continue to be on until given a different command.
// c: 'color' -> set current led strip color, without worrying about current brightness. Goes back to previous command after. Uses 3 bytes from buffer as RGB
// f: 'fade' -> change turn on mode to 'fade in', rather than turning on instantly. next byte is how fast to turn on
// i: 'instant' -> default turn on mode, when touch (or 'on') then set brightness to max with no ramp up
// q: 'queue' -> queue command(s). the buffer will contain a list of commands, terminated in another q. eg
// qc25500oq will queue "color 255 0 0" and then "o"
// @: 'run queue' -> run the next item(s) in the command queue, given a uint8_t number of commands to run

uint8_t capacitiveSensitivity = CAPACITIVE_SENSITIVITY;
const int ledPin = 0;
//const int ledStripPin = 1;
const int capPin = 3;
CRGB leds[NUM_LEDS];
bool touched;
uint8_t touchedCounter = 0;
int capacitiveValue = 0;
volatile int8_t elapsedTime = -1;
volatile uint8_t incrementCounter = 0;
int capacitiveReference = 0;
I2CLedControl i2cHandler(elapsedTime, capacitiveValue, capacitiveReference);
//I2CEchoHandler i2cHandler;

uint8_t debounceDelay = 3;
const uint8_t ledDimSpeed = 8;
uint8_t ledFadeOnSpeed = 8;
uint8_t brightness = 0;
uint8_t ledDimDelay = 10;
uint8_t lastDetected = 0;
char currentCommand = 'r';
char previousCommand = 'r';
char turnOnMode = 'i';
char commandMode = 'N';

struct Command {
    char command;
    byte *data;
};
Command queuedCommands[3]{{'q'},
                          {'q'},
                          {'q'}};
uint8_t commandCursor = 0;

void turnOn();

void fadeOn();

void fadeOff();

void rBehavior();

void oBehavior();

void cBehavior();

void fBehavior();

void iBehavior();

void qBehavior();

void atBehavior(uint8_t numCommands);

void ledSetup();

void setAllLedColor(const CRGB &color);

void configBehavior();

void normalMode();
void configMode();

void timerSetup() {
    TCCR1 = 0; // reset timer1 config
    GTCCR |= (1 || PSR1); // reset prescaler
    TCCR1 = (1 << CTC1) | (1 << CS12) | (1 << CS11); // CTC mode, prescaler = 64
    OCR1C = 124; // Compare value for 1ms
    TIMSK |= (1 << OCIE1A); // enable interrupt

    sei(); // enable global interrupts
}

void setup() {
    SERIAL_BEGIN(9600);
    timerSetup();
    ledSetup();
    i2cHandler.setup();
    pinMode(ledPin, OUTPUT);
    capacitiveReference = ADCTouch.read(capPin, 500);
}

void normalMode() {
    capacitiveValue = ADCTouch.read(capPin, 100) - capacitiveReference;
    if (capacitiveValue > capacitiveSensitivity) {
        SERIAL_PRINTLN("touched");
        if (touchedCounter < debounceDelay) {
            touchedCounter++;
        }
        } else {
            touchedCounter =0;
            touched = false;
        }
        if (touchedCounter == debounceDelay) {
            touched = true;
        }
        switch (currentCommand) {
            case 'r': {
                rBehavior();
                break;
            }
            case 'o': {
                oBehavior();
                break;
            }
            case 'c': {
                cBehavior();
                break;
            }
            case 'f': {
                fBehavior();
                break;
            }
            case 'i': {
                iBehavior();
                break;
            }
            case 'q': {
                qBehavior();
                break;
            }
            case '@': {
                byte* buffer = i2cHandler.getBuffer();
                atBehavior((uint8_t)buffer[0]);
                break;
            }
            case 'C': {
                configBehavior();
                break;
            }
            default:
                break;
        }
    }

void loop() {
    if (i2cHandler.isNewCommand) {
        SERIAL_PRINT("New command: ");
        SERIAL_PRINTLN(i2cHandler.command);
        previousCommand = currentCommand;
        currentCommand = i2cHandler.command;
        i2cHandler.isNewCommand = false;
    }

    if (commandMode == 'N') {
        normalMode();
    } else if (commandMode == 'C') {
        configMode();
    }
    delay(1);
}

void incrementTouchCounter() {
    if (elapsedTime < 127) {
        elapsedTime++;
    }
}

void rBehavior() {
    if (touched) {
        turnOn();
        lastDetected = 0;
    } else if (brightness > 0 && lastDetected > ledDimDelay) {
        fadeOff();
    } else {
        lastDetected++;
    }

    FastLED.setBrightness(brightness);
    FastLED.show();
}

void flashColor( CRGB color, uint16_t delayLength) {
    uint8_t oldBrightness = FastLED.getBrightness();
    CRGB oldColor = leds[0];
    FastLED.setBrightness(255);
    setAllLedColor(color);
    FastLED.show();
    delay(delayLength);
    setAllLedColor(oldColor);
    FastLED.setBrightness(oldBrightness);
    FastLED.show();
}

void configMode() {
    switch (currentCommand) {
        case 'r': {
            flashColor(CRGB::Yellow, 500);
            // turn leds off during calibration
            FastLED.setBrightness(0);
            FastLED.show();
            delay(500);
            int capacitiveReference1 = ADCTouch.read(capPin, 500);
            delay(1000);
            int capacitiveReference2 = ADCTouch.read(capPin, 500);
            delay(1000);
            int capacitiveReference3 = ADCTouch.read(capPin, 500);
            capacitiveReference = (capacitiveReference1 + capacitiveReference2 + capacitiveReference3) / 3;
            currentCommand = previousCommand;
            FastLED.setBrightness(255);
            FastLED.show();
            setAllLedColor(CRGB::Red);
            FastLED.show();
            break;
        }
        case 'N': {
            commandMode = 'N';
            currentCommand = 'r';
            setAllLedColor(CRGB::White);
            FastLED.show();
            break;
        }
        case 's': {
            byte *buffer = i2cHandler.getBuffer();
            capacitiveSensitivity = buffer[0];
            currentCommand = previousCommand;
            break;
        }
        case 'd': {
            flashColor(CRGB::DarkBlue, 500);
            byte *buffer = i2cHandler.getBuffer();
            debounceDelay = buffer[0];
            currentCommand = previousCommand;
            break;
        }
        default:
            break;
    }
}

void oBehavior() {
    turnOn();
    lastDetected = 0;
    FastLED.setBrightness(brightness);
    FastLED.show();
}

void cBehavior() {
    byte *buffer = i2cHandler.getBuffer();
    uint8_t red = buffer[0];
    uint8_t green = buffer[1];
    uint8_t blue = buffer[2];
    setAllLedColor(CRGB(red, green, blue));
    FastLED.show();
    currentCommand = previousCommand;
}

void fBehavior() {
    byte *buffer = i2cHandler.getBuffer();
    ledFadeOnSpeed = buffer[0];
    turnOnMode = 'f';
    currentCommand = previousCommand;
}

void iBehavior() {
    turnOnMode = 'i';
    currentCommand = previousCommand;
}

void qBehavior() {
    byte *buffer = i2cHandler.getBuffer();
    commandCursor = 0;
    uint8_t index = 0;

    for (auto & queuedCommand : queuedCommands) {
        char command = buffer[index++];
        switch (command) {
            case 'q':
                return;
            case ('c'): {
                delete[](queuedCommand.data);
                queuedCommand.data = new byte[3];
                for (uint8_t i = 0; i < 3; i++) {
                    queuedCommand.data[i] = buffer[index++];
                }
                queuedCommand.command = 'c';
                break;
            }
            case 'o': {
                delete[](queuedCommand.data);
                queuedCommand.command = 'o';
                break;
            }
            case 'f': {
                delete[](queuedCommand.data);
                queuedCommand.data = new byte[1]{buffer[index++]};
                queuedCommand.command = 'f';
                break;
            }

            default: {
            }
        }
    }
    currentCommand = previousCommand;
}

void atBehavior(uint8_t numCommands) {
    currentCommand = previousCommand;
    for (uint8_t i = 0; i < numCommands; i++) {
        switch (queuedCommands[commandCursor].command) {
            case 'q':
                return;
            case 'c': {
                setAllLedColor(CRGB(queuedCommands[commandCursor].data[0], queuedCommands[commandCursor].data[1],
                                    queuedCommands[commandCursor].data[2]));
                delete[](queuedCommands[commandCursor].data);
                break;
            }
            case 'o': {
                oBehavior();
                currentCommand = 'o';
            }
            case 'f': {
                ledFadeOnSpeed = queuedCommands[commandCursor].data[0];
                delete[](queuedCommands[commandCursor].data);
                turnOnMode = 'f';
            }

        }
        commandCursor++;
        if (commandCursor == 3) commandCursor = 0;
    }
}

void configBehavior() {
    setAllLedColor(CRGB::Red);
    FastLED.setBrightness(255);
    FastLED.show();
    commandMode = 'C';
    currentCommand = ' ';
}

void ledSetup() {
    FastLED.addLeds<WS2812B, ledStripPin, COLOR_ORDER>(leds, NUM_LEDS);
    setAllLedColor(CRGB::White);
    FastLED.setBrightness(0);


}

void setAllLedColor(const CRGB &color) {
    fill_solid(leds, NUM_LEDS, color);
}

void turnOn() {
    switch (turnOnMode) {
        default:
        case 'o': {
            brightness = 255;
            break;
        }
        case 'f': {
            fadeOn();
        }
    }
}

void fadeOn() {
    if (brightness + ledFadeOnSpeed > 255) {
        brightness = 255;
    } else {
        brightness += ledFadeOnSpeed;
    }
}

void fadeOff() {
    elapsedTime = -1;
    if (brightness < ledDimSpeed) {
        brightness = 0;
    } else {
        brightness -= ledDimSpeed;
    }
}

ISR(TIMER1_COMPA_vect) {
    incrementCounter++;
    if (incrementCounter >= 8) {
        incrementCounter = 0;
        if (touched) {
            incrementTouchCounter();
        }
    }
}