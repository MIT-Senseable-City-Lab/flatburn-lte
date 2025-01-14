#define MODE PWRSAVE

#define AUTOSLEEP TRUE             //Autosleep when inactive
#define INACTIVITY_TIME 420         //Seconds, triggers light sleep
#define DEBUG TRUE
 
#define HW_VERSION V4

#define OPC_ENABLED FALSE           //For Stockholm (Turn off) 
#define IR_ENABLED TRUE
#define CELLULAR_ON_STARTUP TRUE    // TRUE or FALSE
#define DTIME 100  

#define HARVARD_PILOT FALSE         //Is this an Harvard pilot device?
#define OLD_TEMPERATURE_SENSOR FALSE
#define BATT_ENABLED TRUE

#define OPC_DATA_VERSION EXTENDED       // BASE or EXTENDED for full BIN data
#define TCP_GHOSTWRITE FALSE         //For testing purpose, doesn't dump data over TCP but prints it over serial
#define SD_FORMAT_ONSTARTUP FALSE   //Erase SD Card on startup

// Data sampling
#define SAMPLE_RATE 5 //Seconds 
#define VITALS_RATE 30 //Seconds
#define ROUTINE_RATE 60 //seconds

// Data Storage and Broadcasting
#define RECORDS_PER_FILE 200 //standard is 200
#define LOW_BATTERY_THRESHOLD 3.60 //volt


#define TCP_ENDPOINT "3.236.127.62" //MIT BACKEND
