#pragma once

// This will load the definition for common Particle variable types

#include "Particle.h"
#include <Wire.h>

#define V3 0
#define V4 1

// Motherboard pins
#define MB_AN A6
#define MB_CS D23
#define MB_PWM A2

// V4 Power Enable Pins (from schematic page 5 - POWER ENABLE section)
#define EN_3V3_SENSOR D6      // 3V3 TO SENSOR (Q8 SISS27DN)
#define EN_3V3_GPS D5         // 3V3 TO GPS (Q10 SISS27DN)
// NOTE: main_test.cpp.bak used D7 for EN_5V and it worked!
// Either schematic changed or board revision different
#define EN_3V3_QWICC D8       // 3V3 TO QWICC CONNECTOR (swapped with EN_5V)
#define EN_5V D7              // 5V - was D8, changed to D7 per working test

// Legacy aliases for backwards compatibility
#define EN_3V EN_3V3_SENSOR
#define EN_3V_GPS EN_3V3_GPS

// ADC pins
#define BATTERY_VOLTAGE_PIN A5

// Interrupt pins
#define INT_ACC A1            // Accelerometer interrupt (IMU_INT)
#define NOISE_INT_PIN A4      // Noise interrupt (NOISE_INT)

// ADC SPI pins (ADS1256 - from schematic page 9)
#define ADC_nDRDY D16         // ADC data ready
#define ADC_SPI_CS D4         // ADC chip select
#define ADC_SPI_RESET D5      // ADC reset (shared with EN_3V3_GPS)
#define ADC_SPI_SYNC D7       // ADC sync (shared with EN_3V3_QWICC)

// RGB LED pins (TLC59711 - from schematic page 6)
// M.2 pin 43 = D2, M.2 pin 45 = D3 on B-SoM
#define RGB_CLOCK_PIN D2
#define RGB_DATA_PIN D3

// uSD SPI pins (from schematic page 6)
// These use SPI interface on B-SoM
#define USD_SPI_CS SS         // Chip select
#define USD_SPI_SCK SCK       // SPI clock
#define USD_SPI_MOSI MOSI     // SPI MOSI
#define USD_SPI_MISO MISO     // SPI MISO
#define USD_SPI_CD D2         // Card detect (shared with RGB_CLOCK)

// I2C addresses (from schematic)
#define I2C_ADDR_BQ25798 0x6B     // Battery charger (page 4)
#define I2C_ADDR_TPS25730 0x20    // USB PD controller (page 4)
#define I2C_ADDR_ISL28022_VBUS 0x40   // Current sense VBUS (page 5)
#define I2C_ADDR_ISL28022_VSYS 0x45   // Current sense VSYS (page 5)
#define I2C_ADDR_BME280 0x77      // Ambient sensor (page 8)
#define I2C_ADDR_KXTJ3 0x0E       // Accelerometer (page 8)
#define I2C_ADDR_MAX5805 0x1B     // DAC for noise detection (page 8)
#define I2C_ADDR_SPS30 0x69       // PM sensor (page 3)


// This is your main class that users will import into their application
class CS_core
{
public:
  static CS_core &instance()
  {
    if (!_instance)
    {
      _instance = new CS_core();
    }
    return *_instance;
  }

  void begin(uint8_t hw_release);

  // Power rail control
  void enable3V3(bool command);           // Enable 3V3 sensor rail
  void enable3V3_GPS(bool command);       // Enable 3V3 GPS rail
  void enable3V3_QWICC(bool command);     // Enable 3V3 QWICC rail
  void enable5V(bool command);            // Enable 5V rail
  void enableOPC(bool command);           // Enable OPC/PM sensor
  void enableGPS(bool command);           // Enable GPS module
  void activateGPS(bool command);         // Activate GPS
  void enableHEATER(bool command);        // Enable heater
  void enableALL(bool command);           // Enable all power rails

  // Status functions
  uint8_t getGPSstatus();
  uint8_t isCharging();
  uint8_t isCharged();

private:
  CS_core();
  static CS_core* _instance;
  uint8_t m_inp;
  uint8_t m_out;
  uint8_t m_pol;
  uint8_t m_ctrl;
  uint8_t hw_version;
  boolean pinMode_ext(uint8_t address, uint8_t pin, uint8_t mode);
  boolean pinPolarity(uint8_t address, uint8_t pin, uint8_t polarity);
  boolean digitalWrite_ext(uint8_t address, uint8_t pin, boolean val);
  boolean digitalRead_ext(uint8_t address, uint8_t pin);
};

