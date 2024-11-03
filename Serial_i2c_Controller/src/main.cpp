#include <Arduino.h>
#include "Wire.h"

void echo();
void checkLed();
void readModuleSerial();
uint8_t ledStartingAddress = 2;
uint8_t numModules = 2;

void setup() {
    Wire.setClock(100000);
    Wire.begin();
    Serial.begin(9600);
    Serial1.begin(9600);
}

void loop() {
    checkLed();
    //echo();
//    readModuleSerial();
    delay(100);
}

void checkLed() {
for (int address = ledStartingAddress; address < ledStartingAddress + numModules; address++) {
    Wire.requestFrom(address, 1);
    uint8_t isOn = Wire.read();
    if (isOn == 1) {
        Serial.print("LED number ");
        Serial.print(address);
        Serial.println(" is on!");
    }
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