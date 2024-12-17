#include "context.h"

Context context{
        .capacitive_msTouched = 0,
        .capacitive_value = 100,
        .enabledModules = AddressMap(),
        .led = nullptr,
};

