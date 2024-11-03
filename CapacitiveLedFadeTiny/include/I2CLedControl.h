//
// Created by joebr on 10/30/2024.
//

#include "I2CEventHandler.h"


class I2CLedControl : public I2CEventHandler {
public:
    I2CLedControl(bool& isTouched);
    void setup() override;
    void onReceive(int bytesReceived) override;
    void onRequest() override;

private:
    bool& isTouched;
    static I2CLedControl *instance;
protected:
    static void receiveEventWrapper(int bytesReceived);
    static void requestEventWrapper();
};