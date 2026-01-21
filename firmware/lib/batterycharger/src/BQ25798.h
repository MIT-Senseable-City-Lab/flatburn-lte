/*
 * BQ25798 Battery Charger Driver
 * Based on Flatburn V4 schematic - Battery Charger section (page 4)
 * I2C Address: 0x6B
 *
 * Reference: TI BQ25798 Datasheet
 */

#ifndef BQ25798_H
#define BQ25798_H

#include "Particle.h"
#include <Wire.h>

// I2C Address
#define BQ25798_I2C_ADDR 0x6B

// Register Addresses
#define BQ25798_REG_VBAT_LIM        0x00  // Minimal system voltage limit
#define BQ25798_REG_ICHG_LIM        0x02  // Charge current limit
#define BQ25798_REG_VINDPM          0x04  // Input voltage limit
#define BQ25798_REG_IINDPM          0x06  // Input current limit
#define BQ25798_REG_PRECHG_CTRL     0x08  // Precharge control
#define BQ25798_REG_TERM_CTRL       0x09  // Termination control
#define BQ25798_REG_RECHRG_CTRL     0x0A  // Recharge control
#define BQ25798_REG_VOTG_REG        0x0B  // OTG voltage regulation
#define BQ25798_REG_IOTG_REG        0x0D  // OTG current regulation
#define BQ25798_REG_TIMER_CTRL      0x0E  // Timer control
#define BQ25798_REG_CHG_CTRL0       0x0F  // Charger control 0
#define BQ25798_REG_CHG_CTRL1       0x10  // Charger control 1
#define BQ25798_REG_CHG_CTRL2       0x11  // Charger control 2
#define BQ25798_REG_CHG_CTRL3       0x12  // Charger control 3
#define BQ25798_REG_CHG_CTRL4       0x13  // Charger control 4
#define BQ25798_REG_CHG_CTRL5       0x14  // Charger control 5
#define BQ25798_REG_MPPT_CTRL       0x15  // MPPT control
#define BQ25798_REG_TEMP_CTRL       0x16  // Temperature control
#define BQ25798_REG_NTC_CTRL0       0x17  // NTC control 0
#define BQ25798_REG_NTC_CTRL1       0x18  // NTC control 1
#define BQ25798_REG_ICO_LIM         0x19  // ICO current limit
#define BQ25798_REG_CHG_STATUS0     0x1B  // Charger status 0
#define BQ25798_REG_CHG_STATUS1     0x1C  // Charger status 1
#define BQ25798_REG_CHG_STATUS2     0x1D  // Charger status 2
#define BQ25798_REG_CHG_STATUS3     0x1E  // Charger status 3
#define BQ25798_REG_CHG_STATUS4     0x1F  // Charger status 4
#define BQ25798_REG_FAULT_STATUS0   0x20  // Fault status 0
#define BQ25798_REG_FAULT_STATUS1   0x21  // Fault status 1
#define BQ25798_REG_CHG_FLAG0       0x22  // Charger flag 0
#define BQ25798_REG_CHG_FLAG1       0x23  // Charger flag 1
#define BQ25798_REG_CHG_FLAG2       0x24  // Charger flag 2
#define BQ25798_REG_CHG_FLAG3       0x25  // Charger flag 3
#define BQ25798_REG_FAULT_FLAG0     0x26  // Fault flag 0
#define BQ25798_REG_FAULT_FLAG1     0x27  // Fault flag 1
#define BQ25798_REG_ADC_CTRL        0x2E  // ADC control
#define BQ25798_REG_ADC_FUNC_DIS    0x2F  // ADC function disable
#define BQ25798_REG_IBUS_ADC        0x31  // IBUS ADC reading
#define BQ25798_REG_IBAT_ADC        0x33  // IBAT ADC reading
#define BQ25798_REG_VBUS_ADC        0x35  // VBUS ADC reading
#define BQ25798_REG_VAC1_ADC        0x37  // VAC1 ADC reading
#define BQ25798_REG_VAC2_ADC        0x39  // VAC2 ADC reading
#define BQ25798_REG_VBAT_ADC        0x3B  // VBAT ADC reading
#define BQ25798_REG_VSYS_ADC        0x3D  // VSYS ADC reading
#define BQ25798_REG_TS_ADC          0x3F  // TS ADC reading
#define BQ25798_REG_TDIE_ADC        0x41  // TDIE ADC reading
#define BQ25798_REG_PART_INFO       0x48  // Part information

// Charge Status (from CHG_STATUS1)
typedef enum {
    BQ25798_CHG_STAT_NOT_CHARGING = 0x00,
    BQ25798_CHG_STAT_TRICKLE = 0x01,
    BQ25798_CHG_STAT_PRE_CHARGE = 0x02,
    BQ25798_CHG_STAT_FAST_CHARGE = 0x03,
    BQ25798_CHG_STAT_TAPER_CHARGE = 0x04,
    BQ25798_CHG_STAT_TOP_OFF = 0x06,
    BQ25798_CHG_STAT_CHARGE_DONE = 0x07
} BQ25798_ChargeStatus;

// VBUS Status (from CHG_STATUS1)
typedef enum {
    BQ25798_VBUS_STAT_NO_INPUT = 0x00,
    BQ25798_VBUS_STAT_USB_SDP = 0x01,
    BQ25798_VBUS_STAT_USB_CDP = 0x02,
    BQ25798_VBUS_STAT_USB_DCP = 0x03,
    BQ25798_VBUS_STAT_HVDCP = 0x04,
    BQ25798_VBUS_STAT_UNKNOWN = 0x05,
    BQ25798_VBUS_STAT_NON_STD = 0x06,
    BQ25798_VBUS_STAT_OTG = 0x07,
    BQ25798_VBUS_STAT_DIRECT = 0x0B
} BQ25798_VBUSStatus;

class BQ25798 {
public:
    BQ25798();

    bool begin(uint8_t addr = BQ25798_I2C_ADDR);

    // ADC readings
    float getVBAT();          // Battery voltage in V
    float getVBUS();          // Input voltage in V
    float getVSYS();          // System voltage in V
    float getIBUS();          // Input current in mA
    float getIBAT();          // Battery current in mA
    float getTDIE();          // Die temperature in C
    float getTS();            // TS pin voltage percentage

    // Status functions
    bool isCharging();
    bool isCharged();
    bool isVBUSPresent();
    BQ25798_ChargeStatus getChargeStatus();
    BQ25798_VBUSStatus getVBUSStatus();

    // Configuration
    bool enableADC(bool enable);
    bool setChargeCurrent(uint16_t current_mA);
    bool setChargeVoltage(uint16_t voltage_mV);
    bool setInputCurrentLimit(uint16_t current_mA);

    // Part info
    uint8_t getPartNumber();

private:
    uint8_t _addr;

    bool writeRegister(uint8_t reg, uint8_t value);
    bool writeRegister16(uint8_t reg, uint16_t value);
    uint8_t readRegister(uint8_t reg);
    uint16_t readRegister16(uint8_t reg);
};

#endif // BQ25798_H
