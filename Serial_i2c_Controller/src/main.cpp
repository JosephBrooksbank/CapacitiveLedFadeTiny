#include <Arduino.h>
#include "Wire.h"

void echo();
void checkLed();
void checkUserInput();
void readModuleSerial();
void sendCommand(uint8_t address, char command, byte* data, uint8_t messageLength);
void sendCommand(uint8_t address, char command);
void sendGlobalCommand(char command, byte* data, uint8_t messageLength, bool isStickyCommand);
void sendGlobalCommand(char command);
void turnOtherLedsOn();


const uint8_t ledStartingAddress = 2;
const uint8_t numModules = 6;
struct Module {
    bool touched;
    char currentCommand;
    uint8_t address;
};
Module modules[numModules];
byte buffer[30]{};
char currentCommand = ' ';

void setup() {
    Wire.setClock(100000);
    Wire.begin();
    Serial.begin(115200);
    Serial1.begin(9600);
    for (int i = 0; i < numModules; i++) {
        modules[i].touched = false;
        modules[i].currentCommand = 'r';
        modules[i].address = i+ledStartingAddress;
    }
}


void loop() {
    checkUserInput();
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
//        Serial.print("ERROR: Expected value from address ");
//        Serial.print(address);
//        Serial.println(" but it isn't connected!");
    }
}
}

void checkUserInput() {
    if (Serial.available()) {
        if (currentCommand == ' ') {
            currentCommand = Serial.read();
            Serial.print("setting command to ");
            Serial.println(currentCommand);
            Serial.println("waiting for next input...");
        } else {

            switch (currentCommand) {
                case 'c': {
                    if (Serial.available()) {
                        char color = Serial.read();
                        switch (color) {
                            case 'r': {
                                Serial.println("changing color to red");
                                buffer[0] = 255;
                                buffer[1] = 0;
                                buffer[2] = 0;
                                sendGlobalCommand('c', buffer, 3, false);
                            }
                        }
                    }
                    break;
                }
                case 'f': {
                    buffer[0] = Serial.read();
                    Serial.print("Setting turn on mode to fade, with speed ");
                    Serial.println((uint8_t) buffer[0]);
                    sendGlobalCommand('f', buffer, 1, false);
                    break;
                }
                default:
                    Serial.print("Unknown input ");
                    Serial.println(currentCommand);
            }
            currentCommand = ' ';
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

void sendCommand(uint8_t address, char command, byte* data, uint8_t messageLength) {
    Wire.beginTransmission(address);
    Wire.write(command);
    for (int i = 0; i < messageLength; i++) {
        Wire.write(data[i]);
    }
    Wire.endTransmission();
    modules[address - ledStartingAddress].currentCommand = command;
}

void sendCommand(uint8_t address, char command) {
    Wire.beginTransmission(address);
    Wire.write(command);
    Wire.endTransmission();
    modules[address - ledStartingAddress].currentCommand = command;

}

void sendGlobalCommand(char command, byte* data, uint8_t messageLength, bool isStickyCommand) {
    Wire.beginTransmission(0);
    Wire.write(command);
    for (int i = 0; i < messageLength; i++) {
        Wire.write(data[i]);
    }
    Wire.endTransmission();
    if (isStickyCommand){
        for (int i = 0; i < numModules; i++) {
            modules[i].currentCommand = command;
        }
    }
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