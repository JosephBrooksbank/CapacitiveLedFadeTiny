//
// Created by joebr on 10/30/2024.
//


#include "Arduino.h"
#include "config.h"

class I2CLedControl {
public:
    explicit I2CLedControl(volatile int8_t & touchedValue, int & capacitiveValue, int& capacitiveReference);
    void setup();
    void onReceive(int bytesReceived);
    void onRequest() const;
    static void writeInt(int value);
    byte* getBuffer();
    char command;
    bool isNewCommand;

private:
    uint8_t messageLength = 0;
    byte buffer[BUFFER_SIZE]{};
    volatile int8_t & touchedCounter;
    int & capacitiveValue;
    int & capacitiveReference;
    static I2CLedControl *instance;
protected:
    static void receiveEventWrapper(int bytesReceived);
    static void requestEventWrapper();
};