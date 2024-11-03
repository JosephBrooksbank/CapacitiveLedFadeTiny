//
// Created by joebr on 10/30/2024.
//

#ifndef CAPACITIVELEDFADETINY_LOGGER_H
#define CAPACITIVELEDFADETINY_LOGGER_H

#include "Arduino.h"

//#include "EmptyLogger.h"
//#include "SerialLogger.h"

class Logger {
public:
    virtual ~Logger() = default;
    virtual void Setup() = 0;
    virtual void Print(const String &message) = 0;
    virtual void PrintLn(const String &message) = 0;
    virtual void LogInfoLn(const String &message) = 0;
    virtual void LogInfoLn(int message) = 0;
    virtual void LogWarningLn(const String &message) = 0;
    static Logger *GetLogger();
};

#endif //CAPACITIVELEDFADETINY_LOGGER_H

