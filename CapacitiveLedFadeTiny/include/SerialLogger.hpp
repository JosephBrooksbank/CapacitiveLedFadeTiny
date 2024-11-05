#include "Logger.h"
#include "SendOnlySoftwareSerial.h"
#include "config.h"

class SerialLogger : public Logger {
public:
    explicit SerialLogger(): serial(serialTxPin) {}

    void Setup() override {
        serial.begin(baudRate);
    }

    void LogInfoLn(const String &message) override {
        Print("INFO: ");
        serial.println(message);
    }

    void LogInfoLn(int message) override {
        Print("INFO: ");
        serial.println(message);
    }

    void LogWarningLn(const String &message) override {
        Print("WARNING: ");
        serial.println(message);
    }

    void Print(const String &message) override {
        serial.println(message);
    }

    void PrintLn(const String &message) override {
        serial.print(message);
    }

private:
    SendOnlySoftwareSerial serial;
};