//
// Created by joebr on 10/30/2024.
//

#include "I2CEventHandler.hpp"


class I2CLedControl : public I2CEventHandler {
public:
    explicit I2CLedControl(volatile int8_t & touchedValue, int & capacitiveValue, int& capacitiveReference);
    void setup() override;
    void onReceive(int bytesReceived) override;
    void onRequest() override;
    char command;
    bool isNewCommand;

private:
    volatile int8_t & touchedCounter;
    int & capacitiveValue;
    int & capacitiveReference;
    static I2CLedControl *instance;
protected:
    static void receiveEventWrapper(int bytesReceived);
    static void requestEventWrapper();
};