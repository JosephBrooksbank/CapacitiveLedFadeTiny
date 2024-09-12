#include <Arduino.h>
void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);
}

void loop() {
    if (Serial1.available()) {
        char c = char(Serial1.read());
        Serial.print(c);
    }
}