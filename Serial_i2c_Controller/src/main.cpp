#include <Arduino.h>
#include <Wire.h>

void setup() {
    Serial.begin(9600);
    Wire.begin();
}

uint8_t color[] = {254, 149, 73};
void loop() {
    Wire.beginTransmission(4);
    for (int i = 0; i < 3; i++) {
        Wire.write(color[i]);
    }
    Wire.endTransmission();
    Wire.requestFrom(4, 3);
    while (0 < Wire.available()) {
        uint8_t c = Wire.read();
        Serial.print(c);
    }
    Serial.println();
    delay(100);
}