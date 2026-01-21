/*
 * BQ25798 Battery Charger Driver Implementation
 * Based on Flatburn V4 schematic - Battery Charger section (page 4)
 */

#include "BQ25798.h"

BQ25798::BQ25798() {
    _addr = BQ25798_I2C_ADDR;
}

bool BQ25798::begin(uint8_t addr) {
    _addr = addr;

    // Check if device is present
    Wire.beginTransmission(_addr);
    if (Wire.endTransmission() != 0) {
        return false;
    }

    // Enable ADC for continuous conversion
    enableADC(true);

    return true;
}

bool BQ25798::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.write(value);
    return (Wire.endTransmission() == 0);
}

bool BQ25798::writeRegister16(uint8_t reg, uint16_t value) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.write((value >> 8) & 0xFF);  // MSB first
    Wire.write(value & 0xFF);          // LSB
    return (Wire.endTransmission() == 0);
}

uint8_t BQ25798::readRegister(uint8_t reg) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(_addr, (uint8_t)1);
    if (Wire.available()) {
        return Wire.read();
    }
    return 0;
}

uint16_t BQ25798::readRegister16(uint8_t reg) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(_addr, (uint8_t)2);
    if (Wire.available() >= 2) {
        uint16_t value = Wire.read() << 8;  // MSB first
        value |= Wire.read();                // LSB
        return value;
    }
    return 0;
}

bool BQ25798::enableADC(bool enable) {
    uint8_t ctrl = readRegister(BQ25798_REG_ADC_CTRL);
    if (enable) {
        ctrl |= 0x80;  // Enable ADC
        ctrl |= 0x40;  // Continuous conversion
    } else {
        ctrl &= ~0x80;  // Disable ADC
    }
    return writeRegister(BQ25798_REG_ADC_CTRL, ctrl);
}

float BQ25798::getVBAT() {
    // VBAT ADC: 16-bit, LSB = 1mV
    uint16_t raw = readRegister16(BQ25798_REG_VBAT_ADC);
    return raw / 1000.0;  // Convert to V
}

float BQ25798::getVBUS() {
    // VBUS ADC: 16-bit, LSB = 1mV
    uint16_t raw = readRegister16(BQ25798_REG_VBUS_ADC);
    return raw / 1000.0;  // Convert to V
}

float BQ25798::getVSYS() {
    // VSYS ADC: 16-bit, LSB = 1mV
    uint16_t raw = readRegister16(BQ25798_REG_VSYS_ADC);
    return raw / 1000.0;  // Convert to V
}

float BQ25798::getIBUS() {
    // IBUS ADC: 16-bit, LSB = 1mA
    int16_t raw = (int16_t)readRegister16(BQ25798_REG_IBUS_ADC);
    return (float)raw;  // Already in mA
}

float BQ25798::getIBAT() {
    // IBAT ADC: 16-bit signed, LSB = 1mA
    // Positive = discharging, Negative = charging
    int16_t raw = (int16_t)readRegister16(BQ25798_REG_IBAT_ADC);
    return (float)raw;  // Already in mA
}

float BQ25798::getTDIE() {
    // TDIE ADC: 16-bit, LSB = 0.5C
    uint16_t raw = readRegister16(BQ25798_REG_TDIE_ADC);
    return raw * 0.5;  // Convert to C
}

float BQ25798::getTS() {
    // TS ADC: 16-bit, percentage of REGN
    uint16_t raw = readRegister16(BQ25798_REG_TS_ADC);
    return raw * 0.0961;  // Convert to percentage (0.09610% per bit)
}

BQ25798_ChargeStatus BQ25798::getChargeStatus() {
    uint8_t status = readRegister(BQ25798_REG_CHG_STATUS1);
    return (BQ25798_ChargeStatus)((status >> 5) & 0x07);
}

BQ25798_VBUSStatus BQ25798::getVBUSStatus() {
    uint8_t status = readRegister(BQ25798_REG_CHG_STATUS1);
    return (BQ25798_VBUSStatus)(status & 0x0F);
}

bool BQ25798::isCharging() {
    BQ25798_ChargeStatus status = getChargeStatus();
    return (status >= BQ25798_CHG_STAT_TRICKLE && status <= BQ25798_CHG_STAT_TOP_OFF);
}

bool BQ25798::isCharged() {
    return (getChargeStatus() == BQ25798_CHG_STAT_CHARGE_DONE);
}

bool BQ25798::isVBUSPresent() {
    return (getVBUSStatus() != BQ25798_VBUS_STAT_NO_INPUT);
}

bool BQ25798::setChargeCurrent(uint16_t current_mA) {
    // Charge current limit: 10mA to 5000mA, 10mA step
    if (current_mA > 5000) current_mA = 5000;
    uint16_t reg_val = current_mA / 10;
    return writeRegister16(BQ25798_REG_ICHG_LIM, reg_val);
}

bool BQ25798::setChargeVoltage(uint16_t voltage_mV) {
    // Charge voltage limit: 3000mV to 18800mV, 10mV step
    if (voltage_mV < 3000) voltage_mV = 3000;
    if (voltage_mV > 18800) voltage_mV = 18800;
    uint16_t reg_val = (voltage_mV - 3000) / 10;
    return writeRegister16(BQ25798_REG_VBAT_LIM, reg_val);
}

bool BQ25798::setInputCurrentLimit(uint16_t current_mA) {
    // Input current limit: 100mA to 3300mA, 10mA step
    if (current_mA < 100) current_mA = 100;
    if (current_mA > 3300) current_mA = 3300;
    uint16_t reg_val = current_mA / 10;
    return writeRegister16(BQ25798_REG_IINDPM, reg_val);
}

uint8_t BQ25798::getPartNumber() {
    return readRegister(BQ25798_REG_PART_INFO);
}
