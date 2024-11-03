#include <Arduino.h>
#include "Wire.h"

void setup() {
    Wire.setClock(100000);
    Wire.begin();
    Serial.begin(9600);
    Serial1.begin(9600);
}

void loop() {
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

//    if (Serial1.available()) {
//        char c = char(Serial1.read());
//        Serial.print(c);
//    }

    delay(2);
}