#include "Particle.h"
#include "motion_service.h"
#include "CS_core.h"  //

#define LOW_POWER

float   sampleRate = 6.25;  // HZ - Samples per second - 0.781, 1.563, 3.125, 6.25, 12.5, 25, 50, 100, 200, 400, 800, 1600Hz
uint8_t accelRange = 2;     // Accelerometer range = 2, 4, 8, 16g

int32_t result;

KXTJ3 myIMU(0x0E); // Address 0x0E

MotionService *MotionService::_instance = nullptr;
int MotionService::inactivity_counter = 0;
retained uint32_t imu_irq_total = 0;
volatile uint32_t imu_irq_count = 0;
volatile bool imu_irq_flag = false;
uint32_t imu_irq_last_ms = 0;

namespace {
    const uint32_t IMU_HEALTH_MAGIC = 0x494D5548; // "IMUH"
    const int EEPROM_ADDR_IMU_HEALTH = 96;

    struct ImuHealthAlertRecord {
        uint32_t magic;
        uint16_t batt_mv;
        uint16_t reserved;
        uint32_t epoch;
        uint32_t count;
        uint8_t whoami;
        uint8_t status;
        uint16_t pad;
    };

    bool readImuHealthAlert(ImuHealthAlertRecord &rec) {
        EEPROM.get(EEPROM_ADDR_IMU_HEALTH, rec);
        return (rec.magic == IMU_HEALTH_MAGIC);
    }

    void writeImuHealthAlert(uint16_t batt_mv, uint32_t epoch, uint8_t whoami, uint8_t status) {
        ImuHealthAlertRecord rec = {};
        if (readImuHealthAlert(rec)) {
            rec.count++;
        } else {
            rec.magic = IMU_HEALTH_MAGIC;
            rec.count = 1;
        }
        rec.batt_mv = batt_mv;
        rec.epoch = epoch;
        rec.whoami = whoami;
        rec.status = status;
        EEPROM.put(EEPROM_ADDR_IMU_HEALTH, rec);
    }

    void clearImuHealthAlert() {
        ImuHealthAlertRecord rec = {};
        EEPROM.put(EEPROM_ADDR_IMU_HEALTH, rec);
    }
}


void MotionService::timer_fnc(){
   inactivity_counter++;
   Serial.print("Inactivity counter: ");Serial.println(inactivity_counter);
    pinMode(A1, INPUT); // Accelerometer interrupt
}


Timer inactivity_timer(1000, MotionService::timer_fnc);

MotionService::MotionService() {
}
void scanI2Cwire() { // For debugging purposes
    Serial.println("\n----- I2C Scanner -----");

    if (!Wire.isEnabled()) {
        Serial.println("Wire not enabled! Initializing...");
        Wire.begin();
        Wire.setSpeed(CLOCK_SPEED_100KHZ);
        delay(50);
    }
    uint8_t count = 0;
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.printlnf("Found device at 0x%02X on wire", addr);
            count++;
        }
    }
    Serial.printlnf("Found %d device(s)\n", count);
}

// Direct register read function
uint8_t readRegisterDirect(uint8_t addr, uint8_t reg) {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission(false);  // Keep connection open
    Wire.requestFrom(addr, (uint8_t)1);
    if (Wire.available()) {
        return Wire.read();
    }
    return 0xFF;
}

// Direct register write function
void writeRegisterDirect(uint8_t addr, uint8_t reg, uint8_t value) {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

int MotionService::start()
{   
    uint8_t errorAccumulator = 0;
    if (HW_VERSION == V4)
    {

        bool imu_ok = false; // Adjusted IMU start up
        const int MAX_RETRIES = 10; // Added retries in case of failure, latest tests shows consistent boot on first attempt.
                for (int attempt = 1; attempt <= MAX_RETRIES; attempt++)
        {

            Serial.printlnf("\n--- IMU initialization attempt %d/%d ---", attempt, MAX_RETRIES);

            if (!Wire.isEnabled()) { // I was unsure about wire status for most of the boot process, so added a check and init here to be sure.
                Serial.println("Wire not enabled, reinitializing...");
                Wire.begin();
                Wire.setSpeed(CLOCK_SPEED_100KHZ);
                delay(200);
                
                if (!Wire.isEnabled()) {
                    Serial.println("ERROR: Cannot enable Wire!");
                    continue;  // Skip this attempt
                }
            }
            
            if (myIMU.begin(sampleRate, accelRange) == 0)
            {
                Serial.println("IMU initialized successfully!");
                imu_ok = true;
                break;
            }
            
            Serial.printlnf("IMU init attempt %d failed", attempt);
            
            // Recovery actions between attempts
            if (attempt < MAX_RETRIES)
            {
                Serial.println("Attempting recovery...");
                
                // Reset I2C bus
                Wire.begin();
                Wire.setSpeed(CLOCK_SPEED_100KHZ);
                delay(100);
                
                // Longer delay on later attempts
                if (attempt >= 2)
                {
                    Serial.println("Extended delay before retry...");
                    delay(2000);
                }
            }
         }
        
        if (!imu_ok)
        {
            Serial.println("ERROR: IMU failed to initialize after all attempts!");
            errorAccumulator++;
        }
        else
        {
            // Configure interrupt only if IMU initialized successfully
            myIMU.intConf(50, 1, 10, LOW);

            uint8_t readData = 0;
            myIMU.readRegister(&readData, KXTJ3_WHO_AM_I);
            Serial.print("Who am I? 0x");
            Serial.println(readData, HEX);
                        if (readData == 0x35) {
                Serial.println("IMU identity confirmed: KXTJ3-1057"); // Serial confirmation for debugging
            }
        }
    }
    if(errorAccumulator)
    {
        Serial.println("Problem configuring the device.");
    }
    else
    {
        Serial.println("Device O.K.");
    }
    
    attachInterrupt(INT_ACC, MotionService::resetInactivityCounter, FALLING);
    inactivity_timer.start();   
    Serial.println("timer started");
    motionservice_started = true;
    return true;
}

int MotionService::stop()
{
    inactivity_timer.dispose(); 
    motionservice_started = false;
    return 1;
}

int MotionService::waitOnEvent()
{
    return 1;
    // check accelerometer for events and add it to event queque
    // if no motion events for x minutes go to sleep
}

void MotionService::loop()
{
    if (imu_irq_flag) { // IMU debugging can be removed, attempted to fix the inconsistencies.
        imu_irq_flag = false;
        imu_irq_last_ms = millis();
    }

    static uint32_t last_imu_check_ms = 0;
    static uint32_t last_imu_fail_ms = 0;

    // if (Particle.connected()) { // Abroad checking since this was during deployment fase
    //     ImuHealthAlertRecord rec = {};
    //     if (readImuHealthAlert(rec)) {
    //         float hv = rec.batt_mv / 1000.0f;
    //         String payload = String::format("status=fail,who=0x%02X,code=%u,batt_v=%.2f,ts=%lu,count=%lu",
    //                                         rec.whoami,
    //                                         rec.status,
    //                                         hv,
    //                                         rec.epoch,
    //                                         rec.count);
    //         Particle.publish("IMU_HEALTH", payload);
    //         clearImuHealthAlert();
    //     }
    // }

    // if (IMU_HEALTH_CHECK_INTERVAL_SEC > 0) { // Abroad debugging. Did not indicate the issue during deployment sadly.
    //     uint32_t now_ms = millis();
    //     if (now_ms - last_imu_check_ms >= (IMU_HEALTH_CHECK_INTERVAL_SEC * 1000UL)) {
    //         last_imu_check_ms = now_ms;
    //         uint8_t who = 0xFF;
    //         kxtj3_status_t st = myIMU.readRegister(&who, KXTJ3_WHO_AM_I);
    //         bool ok = (st == IMU_SUCCESS && who == 0x35);
    //         if (!ok) {
    //             if (now_ms - last_imu_fail_ms >= (IMU_HEALTH_FAIL_ALERT_INTERVAL_SEC * 1000UL)) {
    //                 last_imu_fail_ms = now_ms;
    //                 if (!CityVitals::instance().BATT_started) {
    //                     CityVitals::instance().startBattery();
    //                 }
    //                 float batt_v = CityVitals::instance().getBatteryVoltage();
    //                 uint16_t batt_mv = (uint16_t)(batt_v * 1000.0f);
    //                 uint32_t epoch = Time.isValid() ? Time.now() : 0;
    //                 writeImuHealthAlert(batt_mv, epoch, who, (uint8_t)st);
    //                 if (Particle.connected()) {
    //                     String payload = String::format("status=fail,who=0x%02X,code=%u,batt_v=%.2f,ts=%lu",
    //                                                     who, (uint8_t)st, batt_v, epoch);
    //                     Particle.publish("IMU_HEALTH", payload);
    //                     clearImuHealthAlert();
    //                 }
    //             }
    //         }
    //     }
    // }

    if(inactivity_counter > INACTIVITY_TIME){
        Serial.println("motion service loop inactivity timer");
        if(AUTOSLEEP && !OVVERRIDE_AUTOSLEEP){
        Serial.println("It's time to get some sleep");
        delay(100);
        resetInactivityCounter();
        CitySleep::instance().stop();
        }
    }
}

void MotionService::resetInactivityCounter(){
    inactivity_counter = 0;
    imu_irq_count++;
    imu_irq_total++;
    imu_irq_flag = true;
}

int MotionService::getInactivityCounter()
{
    return inactivity_counter;
}

uint32_t MotionService::getImuIrqCount()
{
    return imu_irq_count;
}

uint32_t MotionService::getImuIrqTotal()
{
    return imu_irq_total;
}

uint32_t MotionService::getImuIrqLastMs()
{
    return imu_irq_last_ms;
}

void MotionService::setOverrideAutosleep(bool override)
{
    if(override)
        OVVERRIDE_AUTOSLEEP = TRUE;
    else
        OVVERRIDE_AUTOSLEEP = FALSE;
}

void MotionService::testAccelerometer()
{  
  Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.axisAccel( X ), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.axisAccel( Y ), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.axisAccel( Z ), 4);
  /*Serial.print("\nGyroscope:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatGyroX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readFloatGyroY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readFloatGyroZ(), 4);
  Serial.print("\nThermometer:\n");
  Serial.print(" Degrees C = ");
  Serial.println(myIMU.readTempC(), 4);
  Serial.print(" Degrees F = ");
  Serial.println(myIMU.readTempF(), 4);*/
  delay(1000);
}
