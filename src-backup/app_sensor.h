#ifndef HAYAG_APP_SENSOR_H
#define HAYAG_APP_SENSOR_H

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

typedef struct {
    float lux;
    uint8_t relative_light_index;
    bool simulated;
} app_light_reading_t;

esp_err_t app_sensor_init(void);
esp_err_t app_sensor_read(app_light_reading_t *reading);

#endif
