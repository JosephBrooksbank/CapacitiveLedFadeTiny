#include <Arduino.h>
#include "Wire.h"

void echo();
void checkLed();
void readModuleSerial();
void sendCommand(uint8_t address, char command);
void sendGlobalCommand(char command);
void turnOtherLedsOn();
const uint8_t ledStartingAddress = 2;
const uint8_t numModules = 2;
struct Module {
    bool touched;
    char currentCommand;
    uint8_t address;
};
Module modules[numModules];

void setup() {
    Wire.setClock(100000);
    Wire.begin();
    Serial.begin(9600);
    Serial1.begin(9600);
    for (int i = 0; i < numModules; i++) {
        modules[i].touched = false;
        modules[i].currentCommand = 'r';
        modules[i].address = i+ledStartingAddress;
    }
}


void loop() {
    checkLed();
    turnOtherLedsOn();
    //echo();
//    readModuleSerial();
//    delay(1);
}

void checkLed() {
for (int i = 0; i < numModules; i++) {
    int address = modules[i].address;

    Wire.requestFrom(address, 1);
    uint8_t isOn = Wire.read();
    if (isOn == 1) {
        Serial.print("LED number ");
        Serial.print(address);
        Serial.println(" is on!");
        modules[i].touched = true;
    } else if (isOn == 0) {
        modules[i].touched = false;
    } else if (isOn == 255) {
        Serial.print("ERROR: Expected value from address ");
        Serial.print(address);
        Serial.println(" but it isn't connected!");
    }
}
}

void turnOtherLedsOn() {
    bool noneTouched = true;
    for (uint8_t i = 0; i < numModules; i++ ) {
        if (modules[i].touched) {
            noneTouched = false;
            for (uint8_t j = 0; j < numModules; j++) {
                if (j != i) {
                    if (modules[j].currentCommand != 'o') {
                        sendCommand(modules[j].address, 'o');
                    }
                }
            }

        }
    }
    if (noneTouched) {
        sendGlobalCommand('r');
    }
}

void sendCommand(uint8_t address, char command) {
    Wire.beginTransmission(address);
    Wire.write(command);
    Wire.endTransmission();
    modules[address - ledStartingAddress].currentCommand = command;

}

void sendGlobalCommand(char command) {
    Wire.beginTransmission(0);
    Wire.write(command);
    Wire.endTransmission();
    for (int i = 0; i < numModules; i++) {
        modules[i].currentCommand = command;
    }
}

void echo() {
    String message;
    byte n = Serial.available();
    if (n != 0 ) {
        Serial.print("number of bytes available: ");
        Serial.println(Serial.available());

    while (Serial.available()) {
        message = Serial.readString();
    }
    Serial.print("sending ");
    Serial.println(message);

    Wire.beginTransmission(0);
    for (char charToModule : message) {
        Wire.write(charToModule);
    }
    Wire.endTransmission();

    Wire.requestFrom(2, message.length());

    Serial.print("number of bytes available from device: ");
    Serial.println(Wire.available());
    while (1 < Wire.available()) {
        char charFromModule = Wire.read();
        Serial.print(charFromModule);
    }
    char charFromModule = Wire.read();
    Serial.println(charFromModule);
    }
}

void readModuleSerial() {
    while (1 < Serial1.available()) {
        Serial.print(Serial1.read());
    }
    Serial.println(Serial1.read());
}