#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "app_mqtt.h"
#include "app_config.h"
#include "secrets.h"

#include "mqtt_client.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char *TAG = "HAYAG_MQTT";

static esp_mqtt_client_handle_t mqtt_client = NULL;
static bool mqtt_connected = false;

static void mqtt_event_handler(
    void *handler_args,
    esp_event_base_t base,
    int32_t event_id,
    void *event_data
)
{
    esp_mqtt_event_handle_t event =
        (esp_mqtt_event_handle_t)event_data;

    switch ((esp_mqtt_event_id_t)event_id) {

        case MQTT_EVENT_CONNECTED:
            mqtt_connected = true;
            ESP_LOGI(TAG, "MQTT connected");

            esp_mqtt_client_publish(
                mqtt_client,
                HAYAG_MQTT_STATUS_TOPIC,
                "{\"node_id\":\"n02\",\"status\":\"online\"}",
                0,
                1,
                1
            );
            break;

        case MQTT_EVENT_DISCONNECTED:
            mqtt_connected = false;
            ESP_LOGW(TAG, "MQTT disconnected");
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT connection error");
            break;

        default:
            break;
    }

    (void)handler_args;
    (void)base;
    (void)event;
}

esp_err_t app_mqtt_start(void)
{
    esp_mqtt_client_config_t mqtt_config = {
        .broker = {
            .address = {
                .uri = HAYAG_MQTT_BROKER_URI
            }
        }
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_config);

    if (mqtt_client == NULL) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client");
        return ESP_FAIL;
    }

    esp_err_t result = esp_mqtt_client_register_event(
        mqtt_client,
        ESP_EVENT_ANY_ID,
        mqtt_event_handler,
        NULL
    );

    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register MQTT event handler");
        return result;
    }

    return esp_mqtt_client_start(mqtt_client);
}

esp_err_t app_mqtt_publish_reading(
    const app_light_reading_t *reading
)
{
    if (reading == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!mqtt_connected) {
        ESP_LOGW(TAG, "Not connected. Reading not published.");
        return ESP_ERR_INVALID_STATE;
    }

    char payload[256];

    int64_t uptime_ms = esp_timer_get_time() / 1000;

    snprintf(
        payload,
        sizeof(payload),
        "{"
          "\"node_id\":\"%s\","
          "\"sensor\":\"%s\","
          "\"lux\":%.2f,"
          "\"relative_light_index\":%u,"
          "\"mode\":\"%s\","
          "\"uptime_ms\":%lld"
        "}",
        HAYAG_NODE_ID,
        HAYAG_SENSOR_NAME,
        reading->lux,
        reading->relative_light_index,
        reading->simulated ? "simulation" : "hardware",
        (long long)uptime_ms
    );

    int message_id = esp_mqtt_client_publish(
        mqtt_client,
        HAYAG_MQTT_TOPIC,
        payload,
        0,
        1,
        0
    );

    if (message_id < 0) {
        ESP_LOGE(TAG, "MQTT publish failed");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Published: %s", payload);

    return ESP_OK;
}
