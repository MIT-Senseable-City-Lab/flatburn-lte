---
sidebar_position: 7
---

# 💻 Firmware

## Environment

- Code developed using [Particle.io plugin for MS Code](https://www.particle.io/workbench/);
- Devices can be flashed both via cable or cloud flash (data costs);
- Currently testing on bsom [Particle deviceOS@6.1.1](https://docs.particle.io/reference/device-os/firmware);
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
- _CitySense class_ manages data acquisition for environmental sensors (air quality, etc);
- _CityVitals class_ acquires telemetry data (battery status, solar energy production, etc);
- _CityStore class_ manages storing data on the SD card, dumping data over USB;
- _MotionService class_ used to send the device to sleep when the vehicle is not moving;
- _LocationService class_ provides gps data to other classes (e.g. CityStore).

## Operation modes

- _IDLE_ sensors off, provides only telemetry data;
- _REALTIME_ logs data onto the SD card and send it in real time (via Particle publish);
- _LOGGING_ buffer data onto the SD card and send multiple records upon request (via USB);
- _PWRSAVE_ ike LOGGING but in low power mode.

### Payload

_deviceID, timestamp, latitude, longitude, PM1.0, PM 2.5, PM4, PM10, numPM0.5, numPM1, numPM2.5, numPM4, numPM10, PM size, temperature, humidity, gas_op1_w, gas_op1_r, gas_op2_w, gas_op2_r, noise.

### Vitals

_deviceID, timestamp, latitude, longitude, voltage_batt, current_batt, isCharging, isCharginS, isCharged, temp_int, hum_int, voltage_solar, current_solar, cell_strenght_

# Command line interface

A Python script provides a command line interface (CLI) for retrieving data over USB. To use it, follow the step-by-step instructions in the [data retrieval guide](https://mit-senseable-city-lab.github.io/flatburn-lte/deploy/data)
