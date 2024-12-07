#include <Arduino.h>
#include "Wire.h"

void echo();

void checkLed();

void checkUserInput();

void readModuleSerial();

void sendCommand(uint8_t address, char command, byte *data, uint8_t messageLength);

void sendCommand(uint8_t address, char command);

void sendGlobalCommand(char command, byte *data, uint8_t messageLength, bool isStickyCommand);

void sendGlobalCommand(char command);

void turnOtherLedsOn();


const uint8_t ledStartingAddress = 6;
const uint8_t numModules = 1;
struct Module {
    uint16_t touchedCounter;
    char currentCommand;
    uint8_t address;
};
Module modules[numModules];
byte buffer[30]{};
char currentCommand = ' ';

void setup() {
    Wire.setClock(300000);
    Wire.begin();
    Wire.setWireTimeout(1000, true);
    Serial.begin(115200);
    Serial1.begin(9600);
    for (int i = 0; i < numModules; i++) {
        modules[i].touchedCounter = -1;
        modules[i].currentCommand = 'r';
        modules[i].address = i + ledStartingAddress;
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

int readIntFromI2C() {
    int value = 0;
    byte highByte = Wire.read();
    byte lowByte = Wire.read();
    value = (highByte << 8) | lowByte;
    return value;
}

void checkLed() {
    for (int i = 0; i < numModules; i++) {
        int address = modules[i].address;

        Wire.requestFrom(address, 4);
        if (Wire.available()) {
            int8_t scaledCounterFromModule = Wire.read(); // incremented every 8ms touched

            int capacitiveValue = readIntFromI2C();
            uint8_t sensitivity = Wire.read();
            if (scaledCounterFromModule != -1) {
                modules[i].touchedCounter = scaledCounterFromModule * 8;
                Serial.print("Module ");
                Serial.print(address);
                Serial.print(" has been touched for ");
                if (scaledCounterFromModule == 127) {
                    Serial.print("more than 1000ms");
                } else {
                    Serial.print(modules[i].touchedCounter);
                    Serial.print("ms");
                }
                Serial.print(" Capacitive value: ");
                Serial.println(capacitiveValue);
                Serial.print("Sensitivity: ");
                Serial.println(sensitivity);
                buffer[0] = address;
                sendGlobalCommand('t', buffer, 1, false);
            } else {
                buffer[0] = address;
                sendGlobalCommand('u', buffer, 1, false);
                modules[i].touchedCounter = -1;
            }
        }
    }
}

void checkUserInput() {
    if (Serial.available()) {
        String numberString = "";
        int numberValue = 0;
        while (Serial.available()) {
            char inChar = Serial.read();
            Serial.print("inChar: ");
            Serial.println(inChar);
            if (isDigit(inChar)) {
                Serial.print("adding digit: ");
                Serial.println(inChar);
                numberString += (char) inChar;
            }
            else {
                currentCommand = inChar;
            }
        }
        Serial.print("setting numberValue to ");
        Serial.println(numberString);
        numberValue = numberString.toInt();

        Serial.print("command: ");
        Serial.print(currentCommand);
        Serial.print(numberValue);

        switch (currentCommand) {
            case 'q': {
                Serial.println("queuing commands...");
                buffer[0] = 'f';
                buffer[1] = 1;
                buffer[2] = 'c';
                buffer[3] = 0;
                buffer[4] = 255;
                buffer[5] = 0;
                buffer[6] = 'o';
                sendGlobalCommand('q', buffer, 7, false);
                break;
            }
            case '@': {
                Serial.println("sending commands");
                buffer[0] = 3;
                sendGlobalCommand('@', buffer, 1, false);
                currentCommand = ' ';
                break;
            }

            case 'w': {
                buffer[0] = 255;
                buffer[1] = 255;
                buffer[2] = 255;
                sendGlobalCommand('c', buffer, 3, false);
                delay(50);
                sendGlobalCommand('r');
                delay(50);
                sendGlobalCommand('i');
                break;
            }
            default: {
                buffer[0] = numberValue;
                sendGlobalCommand(currentCommand, buffer, 1, false);
            }
        }
    }
}


void turnOtherLedsOn() {
    bool noneTouched = true;
    for (uint8_t i = 0; i < numModules; i++) {
        if (modules[i].touchedCounter != -1) {
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
//        sendGlobalCommand('r');
    }
}

void sendCommand(uint8_t address, char command, byte *data, uint8_t messageLength) {
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

void sendGlobalCommand(char command, byte *data, uint8_t messageLength, bool isStickyCommand) {
    Wire.beginTransmission(0);
    Wire.write(command);
    for (int i = 0; i < messageLength; i++) {
        Wire.write(data[i]);
    }
    Wire.endTransmission();
    if (isStickyCommand) {
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
    if (n != 0) {
        Serial.print("number of bytes available: ");
        Serial.println(Serial.available());

        while (Serial.available()) {
            message = Serial.readString();
        }
        Serial.print("sending ");
        Serial.println(message);

        Wire.beginTransmission(0);
        for (char charToModule: message) {
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