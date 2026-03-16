#define MODE PWRSAVE

#define AUTOSLEEP TRUE            //Autosleep when inactive
#define INACTIVITY_TIME 120         //Seconds, triggers light sleep
// Stationary-triggered sleep/hibernate path removed (reset planned)
#define IMU_HEALTH_CHECK_INTERVAL_SEC 3600  // Check IMU health every hour, can be removed for Brazil if IMU is stable
#define IMU_HEALTH_FAIL_ALERT_INTERVAL_SEC (6 * 60 * 60) // Rate-limit IMU alerts
#define DEBUG TRUE
 
#define HW_VERSION V4
#define CELLULAR_ON_STARTUP FALSE    // TRUE or FALSE
#define DTIME 100  
#define BATT_ENABLED TRUE

#define OPC_DATA_VERSION EXTENDED       // BASE or EXTENDED for full BIN data
#define TCP_GHOSTWRITE FALSE         //For testing purpose, doesn't dump data over TCP but prints it over serial
#define SD_FORMAT_ONSTARTUP FALSE   //Erase SD Card on startup

// Data sampling
#define SAMPLE_RATE 5 //Seconds 
#define VITALS_RATE 300 //Seconds
#define ROUTINE_RATE 60 //seconds

// BME280 watchdog
#define BME_FAIL_REINIT_MS (60UL * 1000UL)      // Try re-init every 60s on failure
#define BME_FAIL_RESET_MS (10UL * 60UL * 1000UL) // Reset after 10 minutes of continuous failure

// Data Storage and Broadcasting
#define RECORDS_PER_FILE 200 //standard is 200
#define LOW_BATTERY_THRESHOLD 3.70 //volt
#define LOW_BATTERY_HIBERNATE FALSE // Keep awake for battery/solar soak testing
#define LOW_BATT_ALERT_INTERVAL_SEC (4 * 60 * 60) // 4 hours

#define TCP_ENDPOINT "127.0.0.1" //change the IP address to dump data over TCP (not implemented yet)

// Battery voltage sanity checks
#define BATT_VOLT_STUCK_DELTA 0.01f
#define BATT_VOLT_STUCK_MS (15UL * 60UL * 1000UL)
#define BATT_VOLT_REINIT_COOLDOWN_MS (180UL * 1000UL)
