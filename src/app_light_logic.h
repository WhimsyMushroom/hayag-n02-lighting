#ifndef HAYAG_APP_LIGHT_LOGIC_H
#define HAYAG_APP_LIGHT_LOGIC_H

#include <stdint.h>

float app_light_calculate_lux(uint16_t channel0, uint16_t channel1);
uint8_t app_light_calculate_relative_index(float lux, float max_lux);

#endif
