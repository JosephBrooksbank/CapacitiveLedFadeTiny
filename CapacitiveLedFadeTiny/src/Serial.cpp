#include "Serial.h"
#include "config.h"

#ifdef USE_SERIAL
#include "SendOnlySoftwareSerial.h"
SendOnlySoftwareSerial mySerial(txPin);  // Define mySerial here
#endif
