#include <Arduino.h>
#include <SendOnlySoftwareSerial.h>
const int ledPin = 0;
const int txPin = 4;


SendOnlySoftwareSerial mySerial (txPin);
void setup() {
    mySerial.begin(9600);
    pinMode(ledPin, OUTPUT);
}


void loop() {
    digitalWrite(ledPin, HIGH);
    mySerial.println("On");
    delay(500);
    digitalWrite(ledPin, LOW);
    mySerial.println("Off");
    delay(500);
}