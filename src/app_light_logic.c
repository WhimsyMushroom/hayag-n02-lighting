#include "app_light_logic.h"
#include <math.h>

float app_light_calculate_lux(uint16_t ch0, uint16_t ch1)
{
    if (ch0 == 0) {
        return 0.0f;
    }

    float ratio = (float)ch1 / (float)ch0;
    float lux = 0.0f;

    if (ratio <= 0.50f) {
        lux = 0.0304f * ch0 -
              0.062f * ch0 * powf(ratio, 1.4f);
    }
    else if (ratio <= 0.61f) {
        lux = 0.0224f * ch0 - 0.031f * ch1;
    }
    else if (ratio <= 0.80f) {
        lux = 0.0128f * ch0 - 0.0153f * ch1;
    }
    else if (ratio <= 1.30f) {
        lux = 0.00146f * ch0 - 0.00112f * ch1;
    }

    if (lux < 0.0f) {
        lux = 0.0f;
    }

    return lux;
}

uint8_t app_light_calculate_relative_index(float lux, float max_lux)
{
    if (max_lux <= 0.0f) {
        return 0;
    }

    float index = (lux / max_lux) * 100.0f;

    if (index < 0.0f) {
        index = 0.0f;
    }

    if (index > 100.0f) {
        index = 100.0f;
    }

    return (uint8_t)(index + 0.5f);
}
