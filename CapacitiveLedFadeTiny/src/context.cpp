#include "context.h"

Context context{
        .mode = NORMAL,
        .led_fadeOnSpeed = 2,
        .led_fadeOffSpeed = 2,
        .led_holdTime = 400,
        .led_state = OFF,
        .capacitive_sensitivity = 10,
        .capacitive_debounce = 5,
        .capacitive_msTouched = 0,
        .capacitive_value = 100,
        .shouldShow = true,
        .ledControl_animation = RAINBOW,
        .enabledModules = AddressMap(),
//        .timingGroup = TimingGroup(6, 500),
        .timingGroups = {
                TimingGroup(I2C_ADDRESS + 1, 500)
        },
        .timingGroupCount = 1,
        .led = nullptr,
};

