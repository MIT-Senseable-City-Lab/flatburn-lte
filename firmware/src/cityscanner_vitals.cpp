#include "cityscanner_vitals.h"
#include "BQ25798.h"
#include "ISL28022.h"
#include "CS_core.h"
#include <math.h>

CityVitals *CityVitals::_instance = nullptr;
BQ25798 battCharger;          // BQ25798 battery charger (I2C: 0x6B)
ISL28022 solarMeter;          // ISL28022 for VBUS current sense (I2C: 0x40)
ISL28022 vsysMeter;           // ISL28022 for VSYS current sense (I2C: 0x45)
FuelGauge fuelg;

namespace {
    float last_vbat = -1.0f;
    uint32_t last_vbat_change_ms = 0;
    uint32_t last_vbat_reinit_ms = 0;

    int socFromVoltage(float batt_voltage) {
        if (batt_voltage > 4.19) {
            return 100;
        } else if (batt_voltage > 4.01) {
            return 80;
        } else if (batt_voltage > 3.86) {
            return 60;
        } else if (batt_voltage > 3.79) {
            return 40;
        } else if (batt_voltage > 3.70) {
            return 20;
        } else if (batt_voltage >= 3.60) {
            return 10;
        } else {
            return 0;
        }
    }
}
// Note: V4 has only one BME280 used for ambient sensing in cityscanner_sense.cpp
// Internal temperature monitoring uses BQ25798 die temperature (getTDIE)

CityVitals::CityVitals() {}


int CityVitals::init()
{
    if(!BATT_started && BATT_ENABLED)
        startBattery();
    delay(DTIME);
    if(!SOLAR_started)
        startSolar();
    delay(DTIME);
    if(!VSYS_started)
        startVSYS();
    delay(DTIME);
    if(!TEMPint_started)
        startTempInt();
    delay(DTIME);
    return 1;
}

int CityVitals::stop_all()
{
    if(BATT_started)
        stopBattery();
    delay(DTIME);
    if(SOLAR_started)
        stopSolar();
    delay(DTIME);
    if(VSYS_started)
        stopVSYS();
    delay(DTIME);
    if(TEMPint_started)
        stopTempInt();
    delay(DTIME);
    return 1;
}

bool CityVitals::startBattery()
{
    // Initialize BQ25798 battery charger
    if (battCharger.begin(I2C_ADDR_BQ25798)) {
        Log.info("BQ25798 battery charger initialized");
        BATT_started = true;
    } else {
        Log.error("BQ25798 battery charger init failed");
        BATT_started = false;
    }
    return BATT_started;
}

bool CityVitals::stopBattery()
{
    BATT_started = false;
    return 1;
}

float CityVitals::getBatteryVoltage()
{
    if (BATT_started) {
        float voltage = battCharger.getVBAT();  // Returns battery voltage in V
        uint32_t now_ms = millis();

        if (last_vbat < 0.0f) {
            last_vbat = voltage;
            last_vbat_change_ms = now_ms;
        } else {
            if (fabsf(voltage - last_vbat) > BATT_VOLT_STUCK_DELTA) {
                last_vbat = voltage;
                last_vbat_change_ms = now_ms;
            } else if ((now_ms - last_vbat_change_ms) >= BATT_VOLT_STUCK_MS) {
                if ((now_ms - last_vbat_reinit_ms) >= BATT_VOLT_REINIT_COOLDOWN_MS) {
                    Log.warn("Battery voltage unchanged for %lu ms, reinitializing charger ADC",
                             (unsigned long)(now_ms - last_vbat_change_ms));
                    last_vbat_reinit_ms = now_ms;
                    // Re-init charger to refresh ADC
                    startBattery();
                    battCharger.enableADC(true);
                    last_vbat_change_ms = now_ms;
                }
            }
        }
        Log.info("Battery voltage: %.2fV", voltage);
        return voltage;
    }
    Log.warn("Battery charger not started, cannot read voltage");
    return 0.0;
}

float CityVitals::getBatteryCurrent()
{
    if (BATT_started) {
        return battCharger.getIBAT();
    }
    return 0.0;
}

float CityVitals::getBatteryTemp()
{
    if (BATT_started) {
        return battCharger.getTDIE();
    }
    return 0.0;
}

float CityVitals::getBatteryVBUS()
{
    if (BATT_started) {
        return battCharger.getVBUS();
    }
    return 0.0;
}

float CityVitals::getBatteryIBUS()
{
    if (BATT_started) {
        return battCharger.getIBUS();
    }
    return 0.0;
}

float CityVitals::getBatteryVSYS()
{
    if (BATT_started) {
        return battCharger.getVSYS();
    }
    return 0.0;
}

float CityVitals::getBatteryTS()
{
    if (BATT_started) {
        return battCharger.getTS();
    }
    return 0.0;
}

uint8_t CityVitals::getChargeStatusCode()
{
    if (BATT_started) {
        return (uint8_t)battCharger.getChargeStatus();
    }
    return 0;
}

uint8_t CityVitals::getVBUSStatusCode()
{
    if (BATT_started) {
        return (uint8_t)battCharger.getVBUSStatus();
    }
    return 0;
}

int CityVitals::getBatterySOC()
{
    if (BATT_started) {
        float batt_voltage = getBatteryVoltage();
        return socFromVoltage(batt_voltage);
    }
    return 0;
}

String CityVitals::getBatteryData() {
    if (BATT_started)
    {
        String battery_data;
        float batt_voltage = getBatteryVoltage();
        int soc = socFromVoltage(batt_voltage);

        // Format: SOC,temperature,voltage,current
        battery_data = String::format("%d,%.1f,%.2f,%.1f",
            soc,
            getBatteryTemp(),
            batt_voltage,
            getBatteryCurrent());

        return battery_data;
    }
    else
    {
        return "na,na,na,na";
    }
}

String CityVitals::getBatteryDetails()
{
    if (!BATT_started) {
        return "na";
    }

    float batt_v = getBatteryVoltage();
    float batt_i = getBatteryCurrent();
    float batt_t = getBatteryTemp();
    int soc = socFromVoltage(batt_v);
    String chg = getChargingStatus();
    float vbus = getBatteryVBUS();
    float ibus = getBatteryIBUS();
    float vsys = getBatteryVSYS();
    float ts = getBatteryTS();
    uint8_t chg_stat = getChargeStatusCode();
    uint8_t vbus_stat = getVBUSStatusCode();

    return String::format(
        "soc=%d,temp=%.1f,batt_v=%.2f,batt_i=%.1f,chg=%s,chg_stat=%u,vbus_stat=%u,vbus=%.2f,ibus=%.1f,vsys=%.2f,ts=%.1f",
        soc,
        batt_t,
        batt_v,
        batt_i,
        chg.c_str(),
        chg_stat,
        vbus_stat,
        vbus,
        ibus,
        vsys,
        ts
    );
}

bool CityVitals::isBatteryLow(){
    float battery_v = getBatteryVoltage();
    Log.info("Battery voltage: %.2f V", battery_v);
    return (battery_v < LOW_BATTERY_THRESHOLD);
}


String CityVitals::getChargingStatus(){
    String charge_status = "0,0,0";

    if (BATT_started) {
        // Use BQ25798 charger status
        bool isCharging = battCharger.isCharging();
        bool inputPresent = battCharger.isVBUSPresent();
        bool isCharged = battCharger.isCharged();

        charge_status = String::format("%d,%d,%d",
            isCharging ? 1 : 0,
            inputPresent ? 1 : 0,
            isCharged ? 1 : 0);
    }

    return charge_status;
}

bool CityVitals::startSolar(){
    // ISL28022 for charger input (VBUS) current sensing at address 0x40
    solarMeter.begin(I2C_ADDR_ISL28022_VBUS, PV_SENSING);
    SOLAR_started = true;
    Log.info("Input meter (ISL28022 @ 0x40) initialized");
    return 1;
}

bool CityVitals::stopSolar(){
    SOLAR_started = false;
    return 1;
}

String CityVitals::getSolarData(){
    if(SOLAR_started)
        return String::format("%.2f,%.1f", solarMeter.getBusVoltage_V(), solarMeter.getCurrent_mA());
    else
        return "na,na";
}

bool CityVitals::startVSYS(){
    // ISL28022 for VSYS current sensing at address 0x45
    vsysMeter.begin(I2C_ADDR_ISL28022_VSYS, BATT_SENSING);
    VSYS_started = true;
    Log.info("VSYS meter (ISL28022 @ 0x45) initialized");
    return 1;
}

bool CityVitals::stopVSYS(){
    VSYS_started = false;
    return 1;
}

String CityVitals::getVSYSData(){
    if(VSYS_started)
        return String::format("%.2f,%.1f", vsysMeter.getBusVoltage_V(), vsysMeter.getCurrent_mA());
    else
        return "na,na";
}

bool CityVitals::startTempInt(){
    // V4 hardware: No separate internal temperature sensor
    // The BME280 is used for ambient sensing in cityscanner_sense.cpp
    // Internal temperature can be read from BQ25798 die temperature if available
    TEMPint_started = true;
    Log.info("Internal temp: using BQ25798 die temperature (if available)");
    return true;
}

bool CityVitals::stopTempInt(){
    TEMPint_started = false;
    return 1;
}

String CityVitals::getTempIntData(){
    // V4: Return BQ25798 die temperature if battery charger is started
    // Otherwise return na - ambient temp is in main sensing payload
    if(BATT_started) {
        float dieTemp = battCharger.getTDIE();
        return String::format("%.2f,na", dieTemp);
    }
    return "na,na";
}

String CityVitals::getSignalStrenght(){
    if(Cellular.isOn()){
        CellularSignal sig = Cellular.RSSI();
        float strength = sig.getStrength();
        return String::format("%.1f", strength);
    }
    else
        return "na";
}

String CityVitals::getVitalsPayload(){
    // Format: SOC,temp_batt,voltage_batt,current_batt,isCharging,inputPresent,isCharged,
    //         temp_int,hum_int,input_voltage,input_current,input_source_code,cell_strength
    String payload = "";

    payload += getBatteryData() + ",";
    payload += getChargingStatus() + ",";
    payload += getTempIntData() + ",";
    payload += getSolarData() + ",";
    payload += String::format("%u,", (unsigned int)getVBUSStatusCode());
    payload += getSignalStrenght();

    return payload;
}
