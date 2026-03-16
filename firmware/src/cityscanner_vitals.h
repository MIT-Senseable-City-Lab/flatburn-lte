#pragma once
#include "cityscanner_config.h"
#include "Particle.h"
// BME280 moved to cityscanner_sense.cpp for ambient sensing


class CityVitals {
    public:
        static CityVitals &instance() {
            if(!_instance) {
                _instance = new CityVitals();
            }
            return *_instance;
        }

      /**
         * @brief Initialize device for application setup()
         *
         * @retval SYSTEM_ERROR_NONE
         */
        int init();
        int stop_all();

        // Battery charger functions (BQ25798)
        bool startBattery(void);
        bool stopBattery(void);
        bool BATT_started = false;
        String getBatteryData(void);
        String getChargingStatus(void);
        bool isBatteryLow();
        float getBatteryVoltage();
        float getBatteryCurrent();      // Battery charge/discharge current
        float getBatteryTemp();         // Battery/charger temperature
        int getBatterySOC();            // Battery SOC estimate (0-100)
        float getBatteryVBUS();
        float getBatteryIBUS();
        float getBatteryVSYS();
        float getBatteryTS();
        uint8_t getChargeStatusCode();
        uint8_t getVBUSStatusCode();
        String getBatteryDetails();

        // Solar/Power monitoring functions (ISL28022)
        bool startSolar(void);
        bool stopSolar(void);
        bool SOLAR_started = false;
        String getSolarData(void);

        // System voltage monitoring (ISL28022 @ 0x45)
        bool startVSYS(void);
        bool stopVSYS(void);
        bool VSYS_started = false;
        String getVSYSData(void);

        // Internal temperature sensor (BME280 @ 0x77)
        bool startTempInt(void);
        bool stopTempInt(void);
        bool TEMPint_started = false;
        String getTempIntData(void);

        // Cellular signal
        String getSignalStrenght();

        // Full vitals payload
        String getVitalsPayload(void);

    private:
        CityVitals();
        static CityVitals* _instance;
};
