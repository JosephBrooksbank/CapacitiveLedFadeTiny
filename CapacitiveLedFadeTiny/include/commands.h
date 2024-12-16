#ifndef COMMANDS_H
#define COMMANDS_H

#include "Arduino.h"
#include "context.h"
#include "AddressMap.h"

// m: set mode for device
void SetMode(byte mode);
// t: toggle another device
void ToggleModule(uint8_t moduleAddress);
// u: untoggle another device
void UnToggleModule(uint8_t moduleAddress);
// g: set timing group
void SetTimingGroup(uint8_t groupIndex, AddressMap&& addresses, uint16_t delay);

#endif