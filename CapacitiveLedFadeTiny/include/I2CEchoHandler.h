//
// Created by joebr on 10/30/2024.
//

#include "I2CEventHandler.h"


class I2CEchoHandler : public I2CEventHandler {
public:
    I2CEchoHandler();
    void setup() override;
    void onReceive(int bytesReceived) override;
    void onRequest() override;

private:
    static I2CEchoHandler *instance;
protected:
    static void receiveEventWrapper(int bytesReceived);
    static void requestEventWrapper();
};
