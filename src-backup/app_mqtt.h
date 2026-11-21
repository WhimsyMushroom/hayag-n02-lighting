#ifndef HAYAG_APP_MQTT_H
#define HAYAG_APP_MQTT_H

#include "esp_err.h"
#include "app_sensor.h"

esp_err_t app_mqtt_start(void);

esp_err_t app_mqtt_publish_reading(
    const app_light_reading_t *reading
);

#endif
