#include "context.h"

Context context {
        .led_fadeOnSpeed = 2,
        .led_fadeOffSpeed = 2,
        .led_holdTime = 400,
        .led_state = OFF,
        .capacitive_sensitivity = 5,
        .capacitive_debounce = 5,
        .capacitive_msTouched = 0,
        .capacitive_value = 100,
        .shouldShow = true
};

