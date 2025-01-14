#include "Particle.h"
#include "cityscanner_sense.h"
#include "CS_core.h"
#include "BME280.h"
#include "sps30.h"
#include <i2c_adc_ads7828.h>
#include <map>
#include <string>

CitySense *CitySense::_instance = nullptr;

constexpr size_t I2C_BUFFER_SIZE = 512;

#define SP30_COMMS I2C_COMMS

SPS30 sps30;
uint8_t ret, error_cnt = 0;
struct sps_values val;
BME280 tempext;
//DFRobot_SHT20 sht20;


// device 0
// Address: A1=0, A0=0
// Command: SD=1, PD1=1, PD0=1
ADS7828 device(0, SINGLE_ENDED | REFERENCE_OFF | ADC_ON, 0x0F);
ADS7828* adc = &device;
ADS7828Channel* adc0 = adc->channel(0);
ADS7828Channel* adc1 = adc->channel(1);
ADS7828Channel* adc2 = adc->channel(2);
ADS7828Channel* adc3 = adc->channel(3);

CitySense::CitySense() {}

int CitySense::init()
{
    if(!TEMPext_started)
        startTEMP();
    delay(DTIME);
    if(!NOISE_started)
        startNOISE();
    delay(DTIME);
    if(!GAS_started)
        startGAS();
    delay(DTIME);
    if(!OPC_started && OPC_ENABLED)
        startOPC();
    delay(DTIME);
    return 1;
}

int CitySense::stop_all()
{
    if(OPC_started)
        stopOPC();
    delay(DTIME);
    if(TEMPext_started)
        stopTEMP();
    delay(DTIME);
    if(NOISE_started)
        stopNOISE();
    delay(DTIME);
    if(GAS_started)
        stopGAS();
    delay(DTIME);
    return 1;
}

HAL_I2C_Config acquireWireBuffer() {
    HAL_I2C_Config config = {
        .size = sizeof(HAL_I2C_Config),
        .version = HAL_I2C_CONFIG_VERSION_1,
        .rx_buffer = new (std::nothrow) uint8_t[I2C_BUFFER_SIZE],
        .rx_buffer_size = I2C_BUFFER_SIZE,
        .tx_buffer = new (std::nothrow) uint8_t[I2C_BUFFER_SIZE],
        .tx_buffer_size = I2C_BUFFER_SIZE
    };
    return config;
}



bool CitySense::startOPC()
{
    CS_core::instance().enableOPC(1);
    if (!sps30.begin(SP30_COMMS))
        Serial.println("could not initialize communication channel.");

    if (!sps30.probe()) {
        Serial.println("could not probe / connect with SPS30.");
        OPC_started = false;
        return 0;
    } else {
        Serial.println(F("Detected SPS30."));
        OPC_started = true;
        return true;
    }
}

bool CitySense::stopOPC()
{
    // Put the SPS30 to sleep
    if (sps30.sleep() != SPS30_ERR_OK) {
        Serial.println("Could not set SPS30 to sleep.");
    }
    OPC_started = false;
    return 1;
}

String CitySense::getOPCdata(int option)
{
    if(OPC_started){
        do
        {
            ret = sps30.GetValues(&val);
            // data might not have been ready
            if (ret == SPS30_ERR_DATALENGTH)
            {
                if (error_cnt++ > 3)
                {
                    Serial.println("Error during reading values: ");
                }
                delay(1500);
            }

            // if other error
            else if (ret != SPS30_ERR_OK)
            {
                Serial.println("Error during reading values: ");
                Serial.println(ret);
                delay(500);
            }

        } while (ret != SPS30_ERR_OK && error_cnt < 10);
        if (ret == SPS30_ERR_OK) {
    error_cnt = 0; // Reset error count if successful
}
        String opcdata = "na";

        opcdata = String::format("%.2f", val.MassPM1) + "," +
              String::format("%.2f", val.MassPM2) + "," +
              String::format("%.2f", val.MassPM4) + "," +
              String::format("%.2f", val.MassPM10) + "," +
              String::format("%.2f", val.NumPM0) + "," +
              String::format("%.2f", val.NumPM1) + "," +
              String::format("%.2f", val.NumPM2) + "," +
              String::format("%.2f", val.NumPM4) + "," +
              String::format("%.2f", val.NumPM10) + "," +
              String::format("%.2f", val.PartSize);

        return opcdata;
    } else {
        return "na,na,na,na,na,na,na,na";
    }
}

bool CitySense::startTEMP()
{
    Wire.begin();
    Serial.println(tempext.beginI2C());
    TEMPext_started = true;
    return 1;
}

bool CitySense::stopTEMP()
{
    TEMPext_started = false;
    return 1;
}

String CitySense::getTEMPdata()
{
    if(TEMPext_started)
    {
        return String::format("%.1f,%.1f", tempext.readTempC(), tempext.readFloatHumidity());
    } else
        return "na,na";
}

bool CitySense::startNOISE(){
    pinMode(A4,INPUT);
    NOISE_started = true;
    return 1;
}
bool CitySense::stopNOISE(){
    NOISE_started = false;
    return 1;
}

String CitySense::getNOISEdata(){
    if(NOISE_started)
        return String(analogRead(A4));
    else
        return "na";
}

bool CitySense::startGAS()
{
    // enable I2C communication
    ADS7828::begin();

    // adjust scaling on an individual channel basis
    adc0->minScale = 220;
    adc0->maxScale = 470;

    adc1->minScale = 220;
    adc1->maxScale = 420;

    adc2->minScale = 270;
    adc2->maxScale = 330;

    adc3->minScale = 270;
    adc3->maxScale = 300;

    GAS_started = true;
    return 1;
}

bool CitySense::stopGAS()
{
    GAS_started = false;
    return 1;
}

String CitySense::getGASdata(){
    if(GAS_started)
    {
        {
            int16_t sn1_w,sn1_r,sn2_w,sn2_r;
            ADS7828::updateAll();
            sn2_w = adc0->value();
            sn2_r = adc1->value();
            sn1_w = adc2->value();
            sn1_r = adc3->value();
            String payload = String::format("%d,%d,%d,%d", sn2_w,sn2_r,sn1_w,sn1_r);
            return payload;
        }
    } else
        return "na,na,na,na";
}

//waking up SPS30

void CitySense::wakeAllSensors()
{
    if (!sps30.wakeup()) {
        Serial.println("Could not wake up SPS30.");
    }
    
    startNOISE();
    startTEMP();
    startGAS();
    delay(1s);
    startOPC();
}

void CitySense::sleepAllSensors()
{
    if (sps30.sleep() != SPS30_ERR_OK) {
        Serial.println("Could not set SPS30 to sleep.");
    }
    
    stopNOISE();
    stopTEMP();
    stopGAS();
    stopOPC();
}