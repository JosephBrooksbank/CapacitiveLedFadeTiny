//
// Created by joebr on 10/30/2024.
//

#include "Logger.h"
#include "Serial.h"

class EmptyLogger : public Logger {

public:
    void Setup() override {
    }

    void Print(const String &message) override {
    }

    void PrintLn(const String &message) override {
    }

    void LogInfoLn(const String &message) override {
    }

    void LogInfoLn(int message) override {
    }

    void LogWarningLn(const String &message) override {
    }

    ~EmptyLogger() override = default;
};
