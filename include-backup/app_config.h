#ifndef HAYAG_APP_CONFIG_H
#define HAYAG_APP_CONFIG_H

#define HAYAG_NODE_ID "n02"
#define HAYAG_SENSOR_NAME "TSL2561"

/*
 * 1 = simulated sensor readings
 * 0 = actual TSL2561 hardware
 */
#define HAYAG_SIMULATION 0

#define HAYAG_I2C_PORT I2C_NUM_0
#define HAYAG_I2C_SDA 21
#define HAYAG_I2C_SCL 22
#define HAYAG_I2C_ADDRESS 0x39

#define HAYAG_LUX_INDEX_MAX 500.0f

#define HAYAG_PUBLISH_INTERVAL_MS 5000

#define HAYAG_MQTT_TOPIC "hayag/n02/telemetry"
#define HAYAG_MQTT_STATUS_TOPIC "hayag/n02/status"

#endif
