
//
// Created by joebr on 10/29/2024.
//

#ifndef CAPACITIVELEDFADETINY_SERIAL_H
#define CAPACITIVELEDFADETINY_SERIAL_H

#include "SendOnlySoftwareSerial.h"

#ifdef USE_SERIAL

extern SendOnlySoftwareSerial mySerial;  // Declare mySerial as an extern variable
#define SERIAL_BEGIN(baud) mySerial.begin(baud)
#define SERIAL_PRINTLN(x) mySerial.println(x)
#define SERIAL_PRINT(x) mySerial.print(x)

#else
#define SERIAL_BEGIN(baud) // Empty
#define SERIAL_PRINTLN(x)  // Empty
#endif

#endif // CAPACITIVELEDFADETINY_SERIAL_H

