#include <string.h>

#include "app_wifi.h"
#include "secrets.h"

#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"

static const char *TAG = "HAYAG_WIFI";

static void wifi_event_handler(
    void *arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void *event_data
)
{
    if (event_base == WIFI_EVENT &&
        event_id == WIFI_EVENT_STA_START) {

        ESP_LOGI(TAG, "Connecting to Wi-Fi...");
        esp_wifi_connect();

    }
    else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {

        ESP_LOGW(TAG, "Wi-Fi disconnected. Retrying...");
        esp_wifi_connect();

    }
    else if (event_base == IP_EVENT &&
             event_id == IP_EVENT_STA_GOT_IP) {

        ip_event_got_ip_t *event =
            (ip_event_got_ip_t *)event_data;

        ESP_LOGI(
            TAG,
            "Connected. IP: " IPSTR,
            IP2STR(&event->ip_info.ip)
        );
    }
}

esp_err_t app_wifi_start(void)
{
    if (strcmp(HAYAG_WIFI_SSID, "YOUR_WIFI_NAME") == 0) {
        ESP_LOGW(TAG, "Configure Wi-Fi credentials in include/secrets.h");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t result = nvs_flash_init();

    if (result == ESP_ERR_NVS_NO_FREE_PAGES ||
        result == ESP_ERR_NVS_NEW_VERSION_FOUND) {

        ESP_ERROR_CHECK(nvs_flash_erase());
        result = nvs_flash_init();
    }

    if (result != ESP_OK) {
        return result;
    }

    result = esp_netif_init();

    if (result != ESP_OK &&
        result != ESP_ERR_INVALID_STATE) {
        return result;
    }

    result = esp_event_loop_create_default();

    if (result != ESP_OK &&
        result != ESP_ERR_INVALID_STATE) {
        return result;
    }

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();

    result = esp_wifi_init(&wifi_config);

    if (result != ESP_OK) {
        return result;
    }

    result = esp_event_handler_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &wifi_event_handler,
        NULL
    );

    if (result != ESP_OK) {
        return result;
    }

    result = esp_event_handler_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &wifi_event_handler,
        NULL
    );

    if (result != ESP_OK) {
        return result;
    }

    wifi_config_t station_config = {0};

    strncpy(
        (char *)station_config.sta.ssid,
        HAYAG_WIFI_SSID,
        sizeof(station_config.sta.ssid) - 1
    );

    strncpy(
        (char *)station_config.sta.password,
        HAYAG_WIFI_PASSWORD,
        sizeof(station_config.sta.password) - 1
    );

    station_config.sta.threshold.authmode =
        WIFI_AUTH_WPA2_PSK;

    result = esp_wifi_set_mode(WIFI_MODE_STA);

    if (result != ESP_OK) {
        return result;
    }

    result = esp_wifi_set_config(
        WIFI_IF_STA,
        &station_config
    );

    if (result != ESP_OK) {
        return result;
    }

    return esp_wifi_start();
}
