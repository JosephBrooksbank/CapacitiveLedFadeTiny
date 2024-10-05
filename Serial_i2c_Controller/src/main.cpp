#include <Arduino.h>
#include <Wire.h>

void setup() {
    Serial.begin(9600);
    Wire.begin();
}

byte x = 0;
void loop() {
    Wire.beginTransmission(4);
    Wire.write('x' );
    Wire.write(x);
    Wire.endTransmission();
    Wire.requestFrom(4, 5);
    while (0 < Wire.available()) {
        char c = Wire.read();
        Serial.print(c);
    }
    Serial.println();
    delay(500);
}