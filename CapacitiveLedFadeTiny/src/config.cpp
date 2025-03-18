
#include "config.h"
#include "EEPROM.h"

Config DefaultConfig = {
    .version = CONFIG_VERSION,
    .touch_sense = DEFAULT_TOUCH_SENSE,
    .off_delay = DEFAULT_OFF_DELAY,
    .max_brightness = DEFAULT_MAX_BRIGHTNESS,
    .color_mode = DEFAULT_COLOR_MODE,
    .single_color_color = DEFAULT_SINGLE_COLOR,
    .fade_amount = DEFAULT_FADE_AMOUNT,
    .hue = DEFAULT_HUE
};

Config get_config() {
    Config conf;
    EEPROM.get(CONFIG_EEPROM_ADDRESS, conf);
    return conf;
}

void set_config(Config &conf) {
    EEPROM.put(CONFIG_EEPROM_ADDRESS, conf);
}

Config setup_config() {
    Config conf;
    EEPROM.get(CONFIG_EEPROM_ADDRESS, conf);
    if (RESET_CONFIG || conf.version != DefaultConfig.version) {
        conf = EEPROM.put(CONFIG_EEPROM_ADDRESS, DefaultConfig);
    }
    return conf;
}