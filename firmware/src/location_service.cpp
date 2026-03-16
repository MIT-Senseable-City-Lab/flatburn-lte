#include "location_service.h"
#include "TinyGPS++.h"
#include "AssetTrackerRK.h"
#include "CS_core.h"
#include "TimeLib.h"
#include "LegacyAdapter.h"
#include <Wire.h>

LocationService *LocationService::_instance = nullptr;
AssetTracker gps;

tmElements_t gpstime;
LocationService::LocationService() {
}

int LocationService::start()
{
    if(!location_started){
        CS_core::instance().enable3V3_GPS(true);
        delay(2000);
        
        // Use UART instead of I2C
        Serial1.begin(9600);
        gps.withSerialPort(Serial1);  // or the correct serial port
        gps.begin();
        gps.startThreadedMode();
        location_started = true;
        Log.info("GPS started on UART, waiting for fix...");
    }
    return 1;
}

int LocationService::stop()
{
    if(location_started)
    {
    CS_core::instance().activateGPS(0);
    CS_core::instance().enableGPS(0);
    CS_core::instance().enable3V3_GPS(false);
    location_started = false;
    }
    return 1;
}
String LocationService::getGPSdata()
{
    if(location_started)
    {
        TinyGPSPlus* tinyGps = gps.getTinyGPSPlus();

        if (!tinyGps->location.isValid() || tinyGps->location.age() > LegacyAdapter::MAX_GPS_AGE_MS) {
            return "na,na";
        }

        float lat = tinyGps->location.lat();   // Use -> for pointer
        float lon = tinyGps->location.lng();   // Use -> for pointer
        
        String gpsdata = String(lat, 6) + "," + String(lon, 6);
        return gpsdata;
    }
    else 
        return "na,na";
}

String LocationService::getGPSDebug()
{
    if (location_started)
    {
        TinyGPSPlus* tinyGps = gps.getTinyGPSPlus();
        uint32_t age = tinyGps->location.age();

        String status = String::format("Chars:%lu,Passed:%lu,Failed:%lu,Fix:%s,Sats:%lu,Age:%lu",
            tinyGps->charsProcessed(),
            tinyGps->passedChecksum(),
            tinyGps->failedChecksum(),
            tinyGps->location.isValid() ? "YES" : "NO",
            tinyGps->satellites.value(),
            age);
        return status;
    }
    else
    {
        return "gps_off";
    }
}
void LocationService::testRawGPS()
{
    Log.info("=== RAW GPS DATA ===");
    
    // Read and print 500 characters
    int count = 0;
    String buffer = "";
    
    while(count < 500 && Serial1.available()) {
        char c = Serial1.read();
        buffer += c;
        count++;
    }
    
    // Print in chunks (Log.info has length limit)
    for(int i = 0; i < buffer.length(); i += 80) {
        Log.info("%s", buffer.substring(i, min(i + 80, (int)buffer.length())).c_str());
    }
    
    Log.info("=== END RAW (%d chars) ===", count);
}



String LocationService::getGPStime()
{
	if (location_started){

		String gps_time =  String(gps.getHour()) +":" + String(gps.getMinute()) +":" + String(gps.getSeconds());
		String gps_date = String(gps.getDay()) +"/" + String(gps.getMonth()) +"/" + String(gps.getYear());
		return gps_date +" "+ gps_time;
	}
	else
	{
		return "00/00/0000";
	}
  
}

String LocationService::getEpochTime(void)
{
    if (location_started)
    {
        TinyGPSPlus* tinyGps = gps.getTinyGPSPlus();
        if (tinyGps->date.isValid() && tinyGps->time.isValid()) {
            tmElements_t gpsTime = {};
            gpsTime.Year = y2kYearToTm(tinyGps->date.year());
            gpsTime.Month = tinyGps->date.month();
            gpsTime.Day = tinyGps->date.day();
            gpsTime.Hour = tinyGps->time.hour();
            gpsTime.Minute = tinyGps->time.minute();
            gpsTime.Second = tinyGps->time.second();
            return String((uint32_t)makeTime(gpsTime));
        }
    }

    if (Time.isValid()) {
        return String((uint32_t)Time.now());
    }

    return "0";
}
