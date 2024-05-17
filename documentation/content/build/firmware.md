---
sidebar_position: 7
---

# 💻 Firmware

## Environment

- Code developed using [Particle.io plugin for MS Code](https://www.particle.io/workbench/);
- Devices can be flashed both via cable or cloud flash (data costs);
- Currently testing on bsom [Particle deviceOS@4.0.0](https://docs.particle.io/reference/device-os/firmware);
- Written in C++ and [Particle deviceOS APIs](https://docs.particle.io/reference/device-os/firmware/);

## Dev Environment Setup

1. Clone [this](https://github.com/MIT-Senseable-City-Lab/flatburn-lte/) repository;
2. Open Particle Workbench;
3. Run the `Particle: Import Project` command, follow the prompts, and wait for the project to load;
4. Run the `Particle: Configure Workspace for Device` command and select a compatible Device OS version and the `bsom` platform when prompted ([docs](https://docs.particle.io/tutorials/developer-tools/workbench/#cloud-build-and-flash));
5. Connect your device;
6. Compile & Flash!

## Quick overview of classes and files

- _cityscanner_config.h_ main config file, needs to be updated before flashing;
- _main.h_ the program startpoint, should not be modified;
- _cityscanner class_ handles operation modes (more below);
- _CitySense class_ manages data acquisition for environemental sensors (air quality, etc);
- _CityVitals class_ acquires telemetry data (battery status, solar energy production, etc);
- _CityStore class_ manages storing data on the SDcard, dumping data over TCP and over Particle Publish methods;
- _MotionService class_ used to send the device to sleep when the vehicle is not moving;
- _LocationService class_ provides gps data to other classes (e.g. CityStore).

## Operation modes

- _IDLE_ sensors off, provides only telemetry data;
- _REALTIME_ logs data onto the SD card and send it in real time (via Particle publish);
- _LOGGING_ buffer data onto the SD card and send multiple records upon request (via TCP);
- _PWRSAVE_ like LOGGING but keeping the cellular modem OFF.

### Payload

_deviceID, timestamp, latitude, longitude, PM1, PM25, PM10, bin0, bin1, bin2, bin3, bin4, bin5, bin6, bin7, bin8, bin9, bin10, bin11, bin12, bin13, bin14, bin15, bin16, bin17, bin18, bin19, bin20, bin21, bin22, bin23, flowrate, countglitch, laser_status, temperature_opc, humidity_opc, data_is_valid, temperature, humidity, ambient_IR, object_IR, gas_op1_w, gas_op1_r, gas_op2_w, gas_op2_r, noise_

### Vitals

_deviceID, timestamp, latitude, longitude, SOC_batt, temp_batt, voltage_batt, voltage_particle, current_batt, isCharging, isCharginS, isCharged, temp_int, hum_int, voltage_solar, current_solar, cell_strenght_

# Command line interface

The CLI is available via REST, Particle.io Console and Slack. Each command might have 0-3 parameters. Some command return via Particle events or serial.

:::tip
Commands are comma-separated.
:::

| Command       | Param.#1     | Param.#2                        | Description                                                                                         |
| :------------ | :----------- | :------------------------------ | :-------------------------------------------------------------------------------------------------- |
| `battery`     |              |                                 | Returns battery `state_of_charge`, `temperature`, `voltage`, `voltage_alt`, `current`, `isCharging` |
| `solar`       |              |                                 | Returns solar panel `voltage`, `current`                                                            |
| `stop`        |              |                                 | Stops the device (light sleep) for 12hours or until it woken up by a motion even                    |
| `hibernate`   | `[duration]` | `seconds`, `minutes` or `hours` | Hibernates the device (heavy sleep)                                                                 |
| `reboot`      |              |                                 | Resets the device to default                                                                        |
| `location`    |              |                                 | Returns the latest known `GPS coordinates`                                                          |
| `device`      | `check`      |                                 | Returns the device ID and last payload, useful for multiple devices                                 |
| `last`        | `payload`    |                                 | Returns the last payload (See payload schema above)                                                 |
| `last`        | `vitals`     |                                 | Returns the last vitals (See vitals schema above)                                                   |
| `sd`          | `files`      |                                 | Returns `n. of files buffered in the SD card`                                                       |
| `sd`          | `dump`       | `all`                           | Dumps all files queued on the SD to mongoDB via TCP                                                 |
| `sd`          | `dump`       | `[files_number]`                | Dumps the number of files passed as parameter to mongoDB via TCP                                    |
| `sd`          | `format`     |                                 | Formats SD card _DO NOT USE_                                                                        |
| `cellularOFF` |              |                                 | Turns off the cellular modem untill the device is manually powercycled                              |
| `autosleep`   | `on`         |                                 | Device goes to sleep after x minutes of no motion                                                   |
| `autosleep`   | `off`        |                                 |                                                                                                     |
| `heat-cool`   | `on`         |                                 | Turns on the heater or the fan                                                                      |
| `heat-cool`   | `off`        |                                 |                                                                                                     |
