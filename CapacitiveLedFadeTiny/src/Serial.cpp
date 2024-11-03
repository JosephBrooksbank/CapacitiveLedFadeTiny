#include "Serial.h"
#include "config.h"
#include "SendOnlySoftwareSerial.h"

#ifdef USE_SERIAL
SendOnlySoftwareSerial mySerial(txPin);  // Define mySerial here
#endif
