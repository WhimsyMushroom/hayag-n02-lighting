#include "app_sensor.h"
#include "app_config.h"
#include "app_light_logic.h"

#if !HAYAG_SIMULATION
#include "driver/i2c.h"
#endif

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "HAYAG_SENSOR";

#if !HAYAG_SIMULATION

static esp_err_t tsl_write_register(uint8_t reg, uint8_t value)
{
    uint8_t data[2] = {
        (uint8_t)(0x80 | reg),
        value
    };

    return i2c_master_write_to_device(
        HAYAG_I2C_PORT,
        HAYAG_I2C_ADDRESS,
        data,
        sizeof(data),
        pdMS_TO_TICKS(1000)
    );
}

static esp_err_t tsl_read_channel(uint8_t reg, uint16_t *value)
{
    uint8_t command = (uint8_t)(0xA0 | reg);
    uint8_t data[2] = {0};

    esp_err_t result = i2c_master_write_read_device(
        HAYAG_I2C_PORT,
        HAYAG_I2C_ADDRESS,
        &command,
        sizeof(command),
        data,
        sizeof(data),
        pdMS_TO_TICKS(1000)
    );

    if (result != ESP_OK) {
        return result;
    }

    *value = (uint16_t)data[0] |
             ((uint16_t)data[1] << 8);

    return ESP_OK;
}

#endif

esp_err_t app_sensor_init(void)
{
#if HAYAG_SIMULATION

    ESP_LOGW(TAG, "SIMULATION MODE ENABLED");
    ESP_LOGW(TAG, "Generated lux values are fictional; no sensor is read.");

    return ESP_OK;

#else

    i2c_config_t config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = HAYAG_I2C_SDA,
        .scl_io_num = HAYAG_I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000
    };

    esp_err_t result = i2c_param_config(HAYAG_I2C_PORT, &config);

    if (result != ESP_OK) {
        return result;
    }

    result = i2c_driver_install(
        HAYAG_I2C_PORT,
        I2C_MODE_MASTER,
        0,
        0,
        0
    );

    if (result != ESP_OK) {
        return result;
    }

    result = tsl_write_register(0x00, 0x03);

    if (result != ESP_OK) {
        return result;
    }

    /* 402 ms integration time. */
    result = tsl_write_register(0x01, 0x02);

    if (result != ESP_OK) {
        return result;
    }

    vTaskDelay(pdMS_TO_TICKS(500));

    ESP_LOGI(TAG, "TSL2561 initialized");

    return ESP_OK;

#endif
}

esp_err_t app_sensor_read(app_light_reading_t *reading)
{
    if (reading == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    float lux = 0.0f;

#if HAYAG_SIMULATION

    /*
     * Fictional values for software testing.
     * The sequence represents changing brightness scenarios,
     * not real environmental measurements.
     */
    static const float simulated_values[] = {
        45.0f,
        75.0f,
        120.0f,
        180.0f,
        250.0f,
        160.0f,
        90.0f
    };

    static unsigned int position = 0;

    const size_t count =
        sizeof(simulated_values) / sizeof(simulated_values[0]);

    lux = simulated_values[position % count];
    position++;

    reading->simulated = true;

#else

    uint16_t channel0 = 0;
    uint16_t channel1 = 0;

    esp_err_t result = tsl_read_channel(0x0C, &channel0);

    if (result != ESP_OK) {
        return result;
    }

    result = tsl_read_channel(0x0E, &channel1);

    if (result != ESP_OK) {
        return result;
    }

    lux = app_light_calculate_lux(channel0, channel1);
    reading->simulated = false;

#endif

    reading->lux = lux;
    reading->relative_light_index =
        app_light_calculate_relative_index(
            lux,
            HAYAG_LUX_INDEX_MAX
        );

    ESP_LOGI(
        TAG,
        "Lux: %.2f | Relative index: %u | Mode: %s",
        reading->lux,
        reading->relative_light_index,
        reading->simulated ? "SIMULATION" : "HARDWARE"
    );

    return ESP_OK;
}
