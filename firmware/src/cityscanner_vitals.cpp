#include "cityscanner_vitals.h"
#include "BQ27200_I2C.h"
#include "ISL28022.h"
#include "CS_core.h"

CityVitals *CityVitals::_instance = nullptr;
BQ27200_I2C batt = BQ27200_I2C(0);
SHTC3 shtc3;
ISL28022 solar;
FuelGauge fuelg; 

CityVitals::CityVitals() {}


int CityVitals::init()
{   if(!BATT_started && BATT_ENABLED)
        startBattery();
    delay(DTIME);
    if(!SOLAR_started)
        startSolar();
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
    if(TEMPint_started)
        stopTempInt();
    delay(DTIME);
    return 1;
}

bool CityVitals::startBattery()
{
    BATT_started = true;
    return 1;
}

bool CityVitals::stopBattery()
{
    BATT_started = false;
    return 1;
}

float CityVitals::getBatteryVoltage()
{
    int batt_volt_adc = analogRead(BATTERY_VOLTAGE_PIN);
    float battery_voltage = (batt_volt_adc / 4095.0) * 3.3 * 2;
    return battery_voltage;
}

String CityVitals::getBatteryData() {
    if (BATT_started)
    {
        String battery_data;
        float batt_voltage = getBatteryVoltage();
        if (batt_voltage > 4.19)
        {
            battery_data = "100";
        }
        else if (batt_voltage > 4.01)
        {
            battery_data = "80";
        }
        else if (batt_voltage > 3.86)
        {
            battery_data = "60";
        }
        else if (batt_voltage > 3.79)
        {
            battery_data = "40";
        }
        else if (batt_voltage > 3.72)
        {
            battery_data = "20";
        }
        else if (batt_voltage > 3.70)
        {
            battery_data = "15";
        }

        battery_data += ",";

        String batteryVoltageStr = String::format("%.2f", getBatteryVoltage());
        battery_data += batteryVoltageStr;
        
        return battery_data;
    }
    else
    {
        return "na,na";
    }

}

bool isBatteryLow(){
    int batt_volt_adc = analogRead(BATTERY_VOLTAGE_PIN);
    float battery_v = (batt_volt_adc / 4095.0) * 3.3 * 2;
    Log.info("Battery voltage:" + String(battery_v));
    if(battery_v < 3.8)
        return true;
    else
        return false;
}


String CityVitals::getChargingStatus(){
    //String charge_status = String::format("%u,%u", CS_core::instance().isCharging(), CS_core::instance().isCharged());

    //Serial.isConnected();
    /*const uint32_t *usbRegStatus = (const uint32_t*)(0x40027470);
    bool usbConnected = (usbRegStatus != 0);
    Serial.print("Usb Connected? : ");
    Serial.println(usbConnected);

    String charge_status = "0";
    if (SOLAR_started)
    {
        if (solar.getBusVoltage_V() > 0)
        {
            charge_status = String::format("%u", 1);
        }
        charge_status += ",";
        if (getBatteryVoltage() > 4.25)
        {
            charge_status.concat(1);
        } 
        else
        {
            charge_status.concat(0);
        }
    }*/

    String charge_status = "0";
    // Check if USB is connected
    const uint32_t *usbRegStatus = (const uint32_t*)(0x40027470);
    bool usbConnected = (usbRegStatus != 0);

    if (SOLAR_started)
    {
        if ((solar.getBusVoltage_V() > 0) && (getBatteryVoltage() < 4.19) && usbConnected)
        {
            charge_status = String::format("%u", 1);
        }
        charge_status += ",";
        if ((solar.getBusVoltage_V() > 0) && (getBatteryVoltage() < 4.19) && !usbConnected)
        {
            charge_status.concat(1);
        }
        else
        {
            charge_status.concat(0);
        }
        charge_status += ",";
        if (getBatteryVoltage() > 4.19)
        {
            charge_status.concat(1);
        } 
        else
        {
            charge_status.concat(0);
        }
    }

    return charge_status;
}

bool CityVitals::startSolar(){
    solar.begin(0x45, PV_SENSING);
    SOLAR_started = true;
    return 1;
}

bool CityVitals::stopSolar(){
    SOLAR_started = false;
    return 1;
}

String CityVitals::getSolarData(){
    if(SOLAR_started)
    return String::format("%.2f",solar.getBusVoltage_V()) + "," + String::format("%.1f",solar.getCurrent_mA());
    else
    return "na,na";
}

bool CityVitals::startTempInt(){
    
    errorDecoder(shtc3.begin());
    TEMPint_started = true;
    return 1;
}

bool CityVitals::stopTempInt(){
    TEMPint_started = false;
    return 1;
}

String CityVitals::getTempIntData(){
    if(TEMPint_started)
    {
        //return String::format("%.1f", temp_internal.readTemperature()) + "," + String::format("%.1f", temp_internal.readHumidity());
        SHTC3_Status_TypeDef result = shtc3.update();
        return String::format("%.2f,%.2f", shtc3.toDegC(), shtc3.toPercent());
    }
    else
    return "na,na";
}

void CityVitals::errorDecoder(SHTC3_Status_TypeDef message)                             // The errorDecoder function prints "SHTC3_Status_TypeDef" resultsin a human-friendly way
{
  switch(message)
  {
    case SHTC3_Status_Nominal : Serial.print("Nominal"); break;
    case SHTC3_Status_Error : Serial.print("Error"); break;
    case SHTC3_Status_CRC_Fail : Serial.print("CRC Fail"); break;
    default : Serial.print("Unknown return code"); break;
  }
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
