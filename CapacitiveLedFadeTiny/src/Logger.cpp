#include "Logger.h"

#ifdef USE_SERIAL
#include "SerialLogger.h"
#endif
#include "EmptyLogger.h"

Logger* Logger::GetLogger() {
#ifdef USE_SERIAL
    return new SerialLogger();
#else
    return new EmptyLogger();
#endif
};
