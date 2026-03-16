#include "Particle.h"
#include "cityscanner_sense.h"
#include "CS_core.h"
#include "BME280.h"
#include "sps30.h"
#include <ADS1256.h>
#include <Adafruit_TLC59711.h>
#include <map>
#include <string>
#include <math.h>


// Define SPI and ADS1256-related pins
#define ADS1256_DRDY_PIN D16   // From ADC_nDRDY
#define ADS1256_CS_PIN D4
#define ADS1256_VREF 2.500f


#define NUM_TLC59711 1
#define RGB_CLOCK_PIN 43
#define RGB_DATA_PIN 45

Adafruit_TLC59711 rgbController(NUM_TLC59711, RGB_CLOCK_PIN, RGB_DATA_PIN);

// Use SPI1, RESET=0, SYNC=0 (those pins are power enables)
ADS1256 ads(ADS1256_DRDY_PIN, 0, 0, ADS1256_CS_PIN, ADS1256_VREF, &SPI1);


CitySense *CitySense::_instance = nullptr;

constexpr size_t I2C_BUFFER_SIZE = 512;

#define SP30_COMMS I2C_COMMS

SPS30 sps30;
uint8_t ret, error_cnt = 0;
struct sps_values val;
BME280 tempext;

retained uint32_t bme_reinit_count = 0;
retained uint32_t bme_reset_count = 0;


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

hal_i2c_config_t acquireWireBuffer() {
    hal_i2c_config_t config = {
        .size = sizeof(hal_i2c_config_t),
        .version = HAL_I2C_CONFIG_VERSION_1,
        .rx_buffer = new (std::nothrow) uint8_t[I2C_BUFFER_SIZE],
        .rx_buffer_size = I2C_BUFFER_SIZE,
        .tx_buffer = new (std::nothrow) uint8_t[I2C_BUFFER_SIZE],
        .tx_buffer_size = I2C_BUFFER_SIZE
    };
    return config;
}

bool CitySense::initI2C()
{
    Serial.println("Configuring I2C bus with 256-byte buffer...");
    Wire.end();
    delay(100);
    
    static uint8_t i2c_rx_buf[256];
    static uint8_t i2c_tx_buf[256];
    
    hal_i2c_config_t config = {
        .size = sizeof(hal_i2c_config_t),
        .version = HAL_I2C_CONFIG_VERSION_1,
        .rx_buffer = i2c_rx_buf,
        .rx_buffer_size = sizeof(i2c_rx_buf),
        .tx_buffer = i2c_tx_buf,
        .tx_buffer_size = sizeof(i2c_tx_buf)
    };
    
    hal_i2c_begin(HAL_I2C_INTERFACE1, I2C_MODE_MASTER, 0x00, &config);
    Wire.setSpeed(CLOCK_SPEED_100KHZ);
    Serial.println("I2C configured");
    return true;
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
    delay(2000);
    
    
    if (!sps30.begin(SP30_COMMS)) {
        Serial.println("Could not initialize SPS30 communication channel.");
        OPC_started = false;
        return false;
    }

    for (int attempt = 0; attempt < 3; attempt++) {
        if (sps30.probe()) {
            Serial.println(F("Detected SPS30."));
            
            sps30.stop();
            delay(100);
            
            if (attempt > 0) {
                Serial.println("Attempting SPS30 reset...");
                sps30.reset();
                delay(2000);
            }
            
            if (sps30.start()) {
                Serial.println("SPS30 measurement started successfully!");
                OPC_started = true;
                delay(1000);
                return true;
            }
            
            Serial.println("Could not start SPS30 measurement");
        }
        
        Serial.printlnf("SPS30 attempt %d failed, retrying...", attempt + 1);
        delay(1000);
    }
    
    Serial.println("Could not start SPS30 after 3 attempts.");
    OPC_started = false;
    return false;
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
        error_cnt = 0;
        do
        {
            ret = sps30.GetValues(&val);

    if (ret != SPS30_ERR_OK)
    {
        error_cnt++;
        delay(200);
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
    //Wire.begin();
    delay(100);
    
    uint8_t result = tempext.beginI2C();
    Serial.printlnf("BME280 beginI2C returned: %d", result);
    
    if (result == 0) {
        Serial.println("BME280 init failed");
        TEMPext_started = false;
        return false;
    }
    
    tempext.readTempC();
    tempext.readFloatHumidity();
    delay(100);
    
    TEMPext_started = true;
    temp_fail_start_ms = 0;
    temp_last_reinit_ms = 0;
    Serial.println("BME280 initialized");
    return true;
}

bool CitySense::stopTEMP()
{
    TEMPext_started = false;
    return 1;
}

String CitySense::getTEMPdata()
{
    if (TEMPext_started)
    {
        float temp_c = tempext.readTempC();
        float humid = tempext.readFloatHumidity();
        if (!isnan(temp_c) && !isnan(humid)) {
            temp_fail_start_ms = 0;
            return String::format("%.1f,%.1f", temp_c, humid);
        }
    }

    uint32_t now = millis();
    if (temp_fail_start_ms == 0) {
        temp_fail_start_ms = now;
    }
    if (now - temp_last_reinit_ms >= BME_FAIL_REINIT_MS) {
        temp_last_reinit_ms = now;
        bme_reinit_count++;
        stopTEMP();
        startTEMP();
    }
    if (now - temp_fail_start_ms >= BME_FAIL_RESET_MS) {
        Log.error("BME280 failed for %lu ms, resetting", (now - temp_fail_start_ms));
        bme_reset_count++;
        System.reset();
    }
    return "na,na";
}

String CitySense::getBMEWatchdogStats()
{
    return String::format("bme_reinit:%lu,bme_reset:%lu", bme_reinit_count, bme_reset_count);
}

bool CitySense::startNOISE(){
    pinMode(A7,INPUT);
    NOISE_started = true;
    return 1;
}
bool CitySense::stopNOISE(){
    NOISE_started = false;
    return 1;
}

String CitySense::getNOISEdata(){
    if(NOISE_started)
        return String(analogRead(A7));
    else
        return "na";
}

bool CitySense::startGAS()
{
    Serial.println("\n----- ADS1256 ADC -----");

    // Check DRDY pin state before init
    pinMode(ADS1256_DRDY_PIN, INPUT);
    Serial.print("ADS1256: DRDY pin state before init: ");
    Serial.println(digitalRead(ADS1256_DRDY_PIN) ? "HIGH" : "LOW");

    // Initialize using library (handles CS, SPI setup, reset, calibration)
    ads.InitializeADC();
    delay(100);

    // Read status register to check if device is responding
    long status = ads.readRegister(STATUS_REG);
    Serial.printlnf("ADS1256 Status: 0x%02lX", status);

    // Check if we got a valid response (0xFF means no device)
    if (status == 0xFF || status == -1) {
        Serial.println("ADS1256: No response (check wiring/power)");
        Serial.println("  - DRDY pin: D16");
        Serial.println("  - CS pin: D4");
        Serial.println("  - SPI1: MOSI/MISO/SCK");
        Serial.print("ADS1256: DRDY pin state now: ");
        Serial.println(digitalRead(ADS1256_DRDY_PIN) ? "HIGH" : "LOW");
        GAS_started = false;
        return false;
    }

    // Configure for gas sensor reading
    ads.setPGA(PGA_1);           // PGA = 1 (+/- 5V range)
    ads.setDRATE(DRATE_10SPS);   // 10 samples per second
    ads.setBuffer(BUFFER_ENABLED); // Enable input buffer for high impedance sensors

    // Read back settings
    Serial.printlnf("ADS1256 PGA: %d", ads.getPGA());
    Serial.printlnf("ADS1256 DRATE: 0x%02lX", ads.readRegister(DRATE_REG));

    GAS_started = true;
    Serial.println("ADS1256: Initialized");
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

    // Read 4 channels (gas sensors) using single-ended inputs
    // MUX values: AINx + AINCOM (GND)
    uint8_t mux_vals[] = {SING_0, SING_1, SING_2, SING_3};
    float voltages[4];

    for (int i = 0; i < 4; i++) {
        ads.setMUX(mux_vals[i]);
        long raw = ads.readSingle();
        voltages[i] = ads.convertToVoltage(raw);
    }

    // SN2_W, SN2_R, SN1_W, SN1_R
    String payload = String::format("%.6f,%.6f,%.6f,%.6f",
        voltages[0], voltages[1], voltages[2], voltages[3]);
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
// Buzzer on A0 (M.2 pin 23 = BUZZER_PWM)
#define BUZZER_PIN A0

void CitySense::beep(uint16_t frequency, uint16_t duration_ms) {
    tone(BUZZER_PIN, frequency, duration_ms);
}

void CitySense::startupBeep() {
    // Three short beeps to indicate startup
    beep(2000, 100);
    delay(150);
    beep(2500, 100);
    delay(150);
    beep(3000, 100);
}
