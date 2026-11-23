# HAYAG · Lighting

- **Proposed node:** `n02`
- **Purpose:** Measure how much light reaches a specific working surface. This node complements HAYAG's general-brightness observations by providing a localized lux measurement. The node reports observations only. It does not diagnose lighting equipment or determine whether a fixture is defective.
- **Owner or co-owner:** Emmery Kelsey Mendoza, Jhanaloden Dipantar
- **Field location:** HAYAG classroom, actual room ID pending
- **Hardware:** ESP32 DevKit / ESP32-WROOM, TSL2561 digital light sensor, and a low-voltage test LED

### I²C configuration

| Signal | ESP32 GPIO |
|---|---:|
| SDA | 21 |
| SCL | 22 |
| I²C address | `0x39` |

The sensor is configured for a 402 ms integration time.

## Software

- PlatformIO
- ESP-IDF
- FreeRTOS
- MQTT

## Node identity

| Setting | Value |
|---|---|
| Node ID | `n02` |
| Sensor | `TSL2561` |
| Location | `assigned-desk` (placeholder until the work area is assigned) |

## MQTT

### Topics

- Telemetry: `hayag/n02/telemetry`
- Status: `hayag/n02/status`

### Telemetry fields

- `node_id`
- `location`
- `sensor`
- `lux`
- `relative_light_index`
- `mode`
- `uptime_ms`

The relative light index is scaled against the configurable 500-lux reference. It is a project-specific relative value, not an official lighting standard or a diagnosis of lighting conditions.

## Simulation and hardware modes

The current default is **software simulation mode** (`HAYAG_SIMULATION 1`). In this mode, the firmware generates fictional lux readings and does not initialize or read a physical TSL2561.

Set `HAYAG_SIMULATION` to `0` when the physical sensor is connected and ready for hardware testing.

Simulation readings are intended to verify the firmware flow and MQTT telemetry. They are not measurements of real lighting conditions.

## Wi-Fi and MQTT configuration

Create `include/secrets.h` using `include/secrets.h.example` as a template. Keep private credentials out of source control.

The example public broker is intended only for temporary testing. Use the team's approved MQTT broker for subsystem integration. Do not publish private information through a public broker.

## Repository Review — Issues, Priorities, and Recommendations

The following items were identified during the repository review. Priorities indicate their relative importance before subsystem integration and hardware deployment.

| Priority | Issue | Recommendation | Status / Notes |
|---|---|---|---|
| **High** | README contained unresolved Git merge-conflict markers. | Consolidate the README into a single, clean document and remove all conflict markers. | Addressed in this cleaned revision. |
| **High** | Simulation mode was disabled even though the README described simulation as the default. | Enable software simulation for the current development stage; switch to hardware mode only after the physical sensor is configured. | Addressed in this cleaned revision (`HAYAG_SIMULATION 1`). |
| **High** | MQTT broker configuration used a temporary/public broker. | Configure the team's approved broker before HAYAG subsystem integration. Avoid sending private or sensitive data through public brokers. | Requires project-specific configuration. |
| **High** | Simulation readings were fixed/generated values and did not represent actual desk lighting. | Use simulated values only to verify firmware flow and telemetry. Test brighter and dimmer scenarios; do not interpret simulated values as real lux measurements. | Documented limitation; simulation behavior can be improved if needed. |
| **Medium** | Sensor initialization and simulation behavior needed to be clearly separated. | Ensure simulation mode does not require physical TSL2561 initialization; hardware mode should report sensor initialization/read failures. | Review and verify during build/testing. |
| **Medium** | Telemetry did not include an assigned work-area location. | Include a `location` field and replace the placeholder once the desk/work area is assigned. | Placeholder included (`assigned-desk`). |
| **Medium** | Status telemetry used a hardcoded node ID. | Use the configured node ID consistently in telemetry and status messages. | Addressed in this cleaned revision. |
| **Medium** | Lux calculation logic was duplicated. | Keep one shared lux-calculation implementation to avoid inconsistencies. | Addressed in this cleaned revision by using the shared light-logic function. |
| **Medium** | Backup directories/files could contain credentials or outdated code. | Exclude backup directories and secrets from Git; remove tracked secrets and rotate credentials if they were exposed. | `.gitignore` updated; check repository history and rotate any exposed credentials. |
| **Medium** | MQTT payload fields and the relative-light index may not match the subsystem's central data registry. | Compare the topic, payload fields, units, reporting interval, and location format against HAYAG's `DATA_REGISTRY.md` before integration. | Pending subsystem verification. |
| **Medium** | Build and simulation behavior had not been verified after the cleanup. | Run the PlatformIO build, then test simulation publishing, MQTT reconnection, and telemetry contents. | Pending verification. |
| **Low** | The relative light index is a project-specific metric that could be misinterpreted. | Keep it clearly documented as a relative value based on the configurable reference, not an official lighting standard or diagnostic result. | Documented. |

### Priority definitions

- **High:** Address before subsystem integration or before relying on the node's output.
- **Medium:** Address or verify during integration and testing.
- **Low:** Documentation or usability improvement that does not block initial testing.

## First test

1. Build and run the firmware in simulation mode.
2. Confirm the generated lux values change over successive readings.
3. Confirm the telemetry is published to `hayag/n02/telemetry`.
4. Verify that the payload contains the node ID, location, sensor name, lux, mode, and uptime.
5. Test brighter and dimmer simulated scenarios.
6. Record results in the project's testing documentation.

## Hardware test — pending

Physical configuration and testing remain pending. When the TSL2561 is connected:

1. Set `HAYAG_SIMULATION` to `0`.
2. Verify the I²C address and wiring.
3. Test under brighter and dimmer conditions.
4. Compare the reported lux against a reference meter if available.
5. Record the actual hardware test results.

## Build

```bash
pio run -e esp32dev
```

## Serial monitor

```bash
pio device monitor -b 115200
```
Log the actual result in [TESTING.md](../../TESTING.md). Store private Wi‑Fi and class broker credentials in the ignored `secrets.h` file when the sketch is ready.
