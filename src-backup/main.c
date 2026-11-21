#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_err.h"

#include "app_config.h"
#include "app_sensor.h"
#include "app_wifi.h"
#include "app_mqtt.h"

static const char *TAG = "HAYAG_N02";

static void sensor_task(void *parameter)
{
    app_light_reading_t reading;

    while (1) {

        esp_err_t result = app_sensor_read(&reading);

        if (result == ESP_OK) {

            ESP_LOGI(
                TAG,
                "Node: %s | Lux: %.2f | Index: %u",
                HAYAG_NODE_ID,
                reading.lux,
                reading.relative_light_index
            );

            app_mqtt_publish_reading(&reading);

        }
        else {
            ESP_LOGE(TAG, "Sensor read failed: %s",
                     esp_err_to_name(result));
        }

        vTaskDelay(
            pdMS_TO_TICKS(HAYAG_PUBLISH_INTERVAL_MS)
        );
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Starting HAYAG Node 2");

#if HAYAG_SIMULATION
    ESP_LOGW(TAG, "Running with simulated sensor data");
#else
    ESP_LOGI(TAG, "Running with physical TSL2561");
#endif

    ESP_ERROR_CHECK(app_sensor_init());

    esp_err_t wifi_result = app_wifi_start();

    if (wifi_result != ESP_OK) {
        ESP_LOGW(
            TAG,
            "Wi-Fi not started: %s",
            esp_err_to_name(wifi_result)
        );
    }

    if (wifi_result == ESP_OK) {

        esp_err_t mqtt_result = app_mqtt_start();

        if (mqtt_result != ESP_OK) {
            ESP_LOGE(
                TAG,
                "MQTT initialization failed: %s",
                esp_err_to_name(mqtt_result)
            );
        }
    }

    xTaskCreate(
        sensor_task,
        "n02_sensor_task",
        4096,
        NULL,
        5,
        NULL
    );
}
