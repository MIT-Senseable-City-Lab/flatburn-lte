#include "Particle.h"
#include "cityscanner_sense.h"
#include "CS_core.h"
#include "BME280.h"
#include "sps30.h"
#include <ADS1256.h>
#include <Adafruit_TLC59711.h>
#include <map>
#include <string>


// Define SPI and ADS1256-related pins
#define ADS1256_DRDY_PIN D16   // From ADC_nDRDY
#define ADS1256_RESET_PIN D5   // Assigned manually
#define ADS1256_SYNC_PIN D7    // Assigned manually
#define ADS1256_CS_PIN D4      // Assigned manually
#define ADS1256_VREF 2.500

#define NUM_TLC59711 1
#define RGB_CLOCK_PIN 43
#define RGB_DATA_PIN 45

Adafruit_TLC59711 rgbController(NUM_TLC59711, RGB_CLOCK_PIN, RGB_DATA_PIN);

ADS1256 ads(ADS1256_DRDY_PIN, ADS1256_RESET_PIN, ADS1256_SYNC_PIN, ADS1256_CS_PIN, ADS1256_VREF, &SPI);


CitySense *CitySense::_instance = nullptr;

constexpr size_t I2C_BUFFER_SIZE = 512;

#define SP30_COMMS I2C_COMMS

SPS30 sps30;
uint8_t ret, error_cnt = 0;
struct sps_values val;
BME280 tempext;


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

bool CitySense::startRGB() {
    if (RGB_started) return true;

    if (!rgbController.begin()) {
        Serial.println("RGB controller failed to begin");
        return false;
    }

    RGB_started = true;
    setRGB(65535, 0, 0); // Turn LED red to confirm it's working
    return true;
}


void CitySense::setRGB(uint16_t r, uint16_t g, uint16_t b) {
    if (!RGB_started) return;
    rgbController.setLED(0, r, g, b);
    rgbController.write();
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
    ads.InitializeADC();              // Core initialization
    ads.setPGA(PGA_1);                // Optional: set gain (PGA_1 to PGA_64)
    ads.setDRATE(DRATE_10SPS);        // Optional: set data rate (see header for options)

    GAS_started = true;
    return true;
}



bool CitySense::stopGAS()
{
    GAS_started = false;
    return 1;
}

String CitySense::getGASdata()
{
    if (!GAS_started) return "na,na,na,na";

    ads.setMUX(SING_0); delay(5); float sn2_w = ads.convertToVoltage(ads.readSingle());
    ads.setMUX(SING_1); delay(5); float sn2_r = ads.convertToVoltage(ads.readSingle());
    ads.setMUX(SING_2); delay(5); float sn1_w = ads.convertToVoltage(ads.readSingle());
    ads.setMUX(SING_3); delay(5); float sn1_r = ads.convertToVoltage(ads.readSingle());

    String payload = String::format("%.6f,%.6f,%.6f,%.6f", sn2_w, sn2_r, sn1_w, sn1_r);
    return payload;
}





//waking up SPS30

void CitySense::wakeAllSensors()
{
    if (!sps30.wakeup()) {
        Serial.println("Could not wake up SPS30.");
    }
    startRGB();
    setRGB(65535, 0, 0); // Red = system awake
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
    setRGB(0, 0, 0);
    stopRGB();
    stopNOISE();
    stopTEMP();
    stopGAS();
    stopOPC();
}
bool CitySense::stopRGB() {
    RGB_started = false;
    return true;
}
