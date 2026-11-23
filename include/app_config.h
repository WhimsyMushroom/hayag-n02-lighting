#ifndef HAYAG_APP_CONFIG_H
#define HAYAG_APP_CONFIG_H

#define HAYAG_NODE_ID "n02"
#define HAYAG_SENSOR_NAME "TSL2561"

/*
 * Development default:
 * 1 = software-generated simulated readings (no physical sensor required)
 * 0 = read the physical TSL2561 over I2C
 *
 * Set to 0 only after the physical sensor is connected and configured.
 */
#define HAYAG_SIMULATION 1

/* Assigned desk/work-area placeholder. Update when the location is finalized. */
#define HAYAG_LOCATION "assigned-desk"

#define HAYAG_I2C_PORT I2C_NUM_0
#define HAYAG_I2C_SDA 21
#define HAYAG_I2C_SCL 22
#define HAYAG_I2C_ADDRESS 0x39

#define HAYAG_LUX_INDEX_MAX 500.0f

#define HAYAG_PUBLISH_INTERVAL_MS 5000

#define HAYAG_MQTT_TOPIC "hayag/n02/telemetry"
#define HAYAG_MQTT_STATUS_TOPIC "hayag/n02/status"

#endif
