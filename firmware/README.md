# Flatburn-firmware

## Environment
- Code developed using [Particle.io plugin for MS Code](https://www.particle.io/workbench/)
- Devices can be flashed both via cable or cloud flash (data costs)
- Compatible with [Particle deviceOS@6.2.1](https://docs.particle.io/reference/device-os/firmware)
- Written in C++ and [Particle deviceOS APIs](https://docs.particle.io/reference/device-os/firmware/)

## Dev Environment Setup

1. Clone this repository
2. Open Particle Workbench
3. Run the `Particle: Import Project` command, follow the prompts, and wait for the project to load
4. Run the `Particle: Configure Workspace for Device` command and select a compatible Device OS version and the `bsom` platform when prompted ([docs](https://docs.particle.io/tutorials/developer-tools/workbench/#cloud-build-and-flash))
5. Connect your device
6. Compile & Flash!

## Quick overview of classes and files
- *cityscanner_config.h* main config file, needs to be updated before flashing
- *main.h* the program startpoint, should not be modified
- *cityscanner class* handles operation modes (more below)
- *CitySense class* manages data acquisition for environmental sensors (air quality, etc)
- *CityVitals class* acquires telemetry data (battery status, solar energy production, etc)
- *CityStore class* manages storing data on the SD card, dumping data over USB
- *MotionService class* used to send the device to sleep when the vehicle is not moving
- *LocationService class* provides GPS data to other classes (e.g. CityStore)

## Operation modes
- *IDLE* sensors off, provides only telemetry data
- *REALTIME* logs data onto the SD card
- *LOGGING* buffer data onto the SD card and send multiple records upon request (via USB)
- *PWRSAVE* like LOGGING but in low power mode

#### Payload

deviceID, timestamp, latitude, longitude, PM1.0, PM 2.5, PM4, PM10, numPM0.5, numPM1, numPM2.5, numPM4, numPM10, PM size, temperature, humidity, gas_op1_w, gas_op1_r, gas_op2_w, gas_op2_r, noise

### Vitals
deviceID, time, latitude, longitude, SOC_batt, temp_batt, voltage_batt, voltage_particle, current_batt, isCharging, isCharginS, temp_int, hum_int, voltage_solar, current_solar, cell_strenght

# Command line interface
The CLI is available via Python script to retrieve the data over USB

Command | Parameter #1 | Parameter #2 | Parameter #3 | Description
--------|--------------|--------------|--------------|-------------
battery |              |              |               | returns battery state_of_charge,temperature,voltage,voltage_alt,current,isCharging
solar   |              |              |               | returns solar panel voltage,current 
stop | || |Stops the device (light sleep) for 12hours or until it woken up by a motion even 
hibernate | [duration] | seconds OR minutes OR hours| Hibernate the device (heavy sleep)
reboot  | | | | Resets the device to default
location | | | | Returns the latest known GPS coordinates
device | check | | Return the device ID and last payload, useful for multiple devices
last | payload | | Return the last payload (See payload schema above)
last | vitals | |  Return the last vitals (See vitals schema above)
sd | files | | | Returns n. of files buffered in the SD card
sd | dump | all | | Dump all files queued on the SD to mongoDB via TCP
sd | dump | [files_number]] | | Dump the number of files passed as parameter to mongoDB via TCP
sd | format | | | Format SD card *DO NOT USE*
cellularOFF | | | | Turns off the cellular modem untill the device is manually powercycled 
autosleep | on | | | Device goes to sleep after x minutes of no montion
autosleep | off | | | 

