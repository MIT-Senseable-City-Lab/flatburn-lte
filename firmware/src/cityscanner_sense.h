#pragma once
#include "cityscanner_CONFIG.h"
#include "Particle.h"
#define BASE 0 
#define EXTENDED 1 

class CitySense {
    public:
        static CitySense &instance() {
            if(!_instance) {
                _instance = new CitySense();
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
        bool startOPC(void);
        bool stopOPC(void);
        bool OPC_started = false;
        String last_opc_data = "na";
        String getOPCdata(int option);
        bool startTEMP(void);
        bool stopTEMP(void); 
        bool TEMPext_started = false;
        String getTEMPdata(void);
        bool startNOISE(void);
        bool stopNOISE(void);
        bool NOISE_started = false;
        String getNOISEdata(void);
        bool startGAS(void);
        bool stopGAS(void);
        bool GAS_started = false;
        String getGASdata(void);
        void wakeAllSensors();
        void sleepAllSensors();
        bool startRGB(void);
        bool stopRGB(void);
        void setRGB(uint16_t r, uint16_t g, uint16_t b);

        bool RGB_started = false;


    private:
        CitySense();
        static CitySense* _instance;
};
