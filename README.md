# HAYAG Node 2 - Ambient Light Monitoring

## Hardware
- ESP32-WROOM / ESP32 DevKit
- TSL2561 digital ambient light sensor

## Software
- PlatformIO
- ESP-IDF
- MQTT
- FreeRTOS

## Node identity
n02

## MQTT topics
- hayag/n02/telemetry
- hayag/n02/status

## Telemetry fields
- node_id
- sensor
- lux
- relative_light_index
- mode
- uptime_ms

## Simulation mode
The default configuration uses fictional simulated readings.

Set HAYAG_SIMULATION to 0 in include/app_config.h
when the physical TSL2561 is connected.

## Physical sensor
Default I2C address: 0x39
SDA: GPIO21
SCL: GPIO22

The TSL2561 integration time is configured for 402 ms.
The relative index is scaled against a configurable
500 lux reference and is not an official lighting standard.

## Wi-Fi and MQTT
Configure include/secrets.h.

The included public MQTT broker is for temporary testing
only. Use the team's approved broker for integration.
Never publish private information through a public broker.

## Build
pio run -e esp32dev

## Serial monitor
pio device monitor -b 115200
