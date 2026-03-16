#pragma once
#include "CS_core.h"
#include "Particle.h"
#include "location_service.h"
#include "cityscanner_sense.h"
#include "cityscanner_vitals.h"
#include "cityscanner_store.h"
#include "cityscanner_sleep.h"
#include "cityscanner.h"


int commandLine(String command);

int initCLI()
{
  Log.info("CLI started");
  Particle.function("CLI", commandLine);
  return 1;
}

void serialEvent()
{
  String s = "na";
  s = Serial.readStringUntil('\n');  // Accept newline (Python script uses \n)
  s.trim();  // Remove any trailing \r or whitespace
  if (s.length() > 0) {
    commandLine(s);
  }
}

int commandLine(String command)
{
  int index = command.indexOf(',');
  String first_parameter = "na";
  String second_parameter = "na";
  String third_parameter = "na";
  String fourth_parameter = "na";

  if (index == -1)
  {
    first_parameter = command;
  }
  else if (index > 0)
  {
    int secondCommaIndex = command.indexOf(',', index + 1);
    int thirdCommaIndex = command.indexOf(',', secondCommaIndex + 1);
    first_parameter = command.substring(0, index);
    second_parameter = command.substring(index + 1, secondCommaIndex);
    third_parameter = "na";
    fourth_parameter = "na";
    if (secondCommaIndex > 0 && thirdCommaIndex > 0)
    {
      third_parameter = command.substring(secondCommaIndex + 1, thirdCommaIndex);
      fourth_parameter = command.substring(thirdCommaIndex + 1);
    }
    else if (secondCommaIndex > 0)
    {
      third_parameter = command.substring(secondCommaIndex + 1);
    }
  }

  Log.info("RECEIVED: " + first_parameter + "," + second_parameter + "," + third_parameter + "," + fourth_parameter);
  
  //FUNCTION CALLS
  // Serial download commands (for Python script)
  if (!first_parameter.compareTo("LIST"))
  {
    CityStore::instance().listAllFiles();
  }
  else if (first_parameter.startsWith("READ:"))
  {
    String filename = first_parameter.substring(5); // Remove "READ:" prefix
    CityStore::instance().readFileToSerial(filename.c_str());
  }
  else if (!first_parameter.compareTo("DELETE_ALL"))
  {
    CityStore::instance().deleteAllForDownload();
  }
  else if (!first_parameter.compareTo("reboot"))
  {
    System.reset();
  }
  else if (!first_parameter.compareTo("dfu"))
  {
    Log.info("Entering DFU mode");
    if (Particle.connected())
      Particle.publish("DFU", "entering");
    delay(200);
    System.dfu();
  }
  else if (!first_parameter.compareTo("stop"))
  {
    CitySleep::instance().stop();  // for 12 hours or WKP pin triggered
  }
  else if (!first_parameter.compareTo("hibernate"))
  {
    if(!third_parameter.compareTo("seconds"))
      CitySleep::instance().hibernate(second_parameter.toInt(),SECONDS);
    else if(!third_parameter.compareTo("minutes"))
      CitySleep::instance().hibernate(second_parameter.toInt(),MINUTES);
    else if(!third_parameter.compareTo("hours"))
      CitySleep::instance().hibernate(second_parameter.toInt(),HOURS);
  }

  else if (!first_parameter.compareTo("sd"))
  {
    if (!second_parameter.compareTo("dump")) 
    {
      if(!third_parameter.compareTo("all")) 
      {
        Log.info("Dumping All Data Files");
        CityStore::instance().dumpData(ALL_FILES);
        if (Particle.connected())
        Particle.publish("DUMP", "ended_dumping_all_data");
      }
      else
      {
        Log.info("Dumping Some Data Files");
        CityStore::instance().dumpData(third_parameter.toInt());
        if (Particle.connected())
          Particle.publish("DUMP", "ended_dumping_" + String(third_parameter.toInt()) + "_files");
      }
    } 
    else if (!second_parameter.compareTo("files"))
    {
      String files_in_queue = String::format("%u", CityStore::instance().countFilesInQueue());
      Log.info(files_in_queue);
      if (Particle.connected())
        Particle.publish("FILES", files_in_queue);
    }
    else if (!second_parameter.compareTo("health")) // SD card health status for debugging
    {
      String status = CityStore::instance().getSDHealth();
      Log.info(status);
      if (Particle.connected())
        Particle.publish("SDHEALTH", status);
    }
    else if (!second_parameter.compareTo("reinit")) // SD card reboot for remote management
    {
      CityStore::instance().init();
      String status = CityStore::instance().getSDHealth();
      Log.info(status);
      if (Particle.connected())
        Particle.publish("SDHEALTH", status);
    }


    else if (!second_parameter.compareTo("format"))
    {
      CityStore::instance().reInit();
      Log.info("Format SD");
      if (Particle.connected())
        Particle.publish("Format SD", "SD card initialized");
    }
  }

  else if (!first_parameter.compareTo("autosleep")) // Remote control of the autosleep function
  {
    if (!second_parameter.compareTo("on")){   
      MotionService::instance().setOverrideAutosleep(FALSE);
      Cityscanner::instance().sendWarning("Autosleep ON");
  }
  else if (!second_parameter.compareTo("off")){
      MotionService::instance().setOverrideAutosleep(TRUE);
      Cityscanner::instance().sendWarning("Autosleep OFF");
  }
  }

  else if (!first_parameter.compareTo("heat-cool")) // Heater is outdated? Can be removed?
  {
    if (!second_parameter.compareTo("on")){   
      CS_core::instance().enableHEATER(TRUE);
      Cityscanner::instance().sendWarning("heat-cool ON");
  }
  else if (!second_parameter.compareTo("off")){
      CS_core::instance().enableHEATER(FALSE);
      Cityscanner::instance().sendWarning("heat-cool OFF");
  }
  }

  else if (!first_parameter.compareTo("last"))
  {
    String response = "error";
    if (!second_parameter.compareTo("payload"))
    {
      response = Time.now();
      response += "," + LocationService::instance().getGPSdata() + ",";
      response += Cityscanner::instance().data_payload;
      Log.info(response);
      if (Particle.connected())
          Particle.publish("last_payload", response);
    }
    else if (!second_parameter.compareTo("vitals"))
    {
      response = Time.now();
      response += "," + LocationService::instance().getGPSdata() + ",";
      response += Cityscanner::instance().vitals_payload;
      Log.info(response);
      if (Particle.connected())
          Particle.publish("last_vitals", response);
    }
  }

else if (!first_parameter.compareTo("device-check"))
{
    // Construct the initial part of the payload with device ID, epoch time, and GPS data
   String response = System.deviceID() + "," + String(LocationService::instance().getEpochTime()) + "," + LocationService::instance().getGPSdata() + "," + CitySense::instance().getOPCdata(BASE) + "," + CitySense::instance().getTEMPdata() + "," + CitySense::instance().getGASdata() + "," + CitySense::instance().getNOISEdata();
    // Publish the response if the device is connected
    if (Particle.connected())
    {
        Particle.publish("device", response);
    }
}

else if (!first_parameter.compareTo("location"))
  {
    String status = "na";
    status = LocationService::instance().getGPSdata();
    Log.info(status);
    if (Particle.connected())
      Particle.publish("GPS", status);
  }
  else if (!first_parameter.compareTo("gpsdebug")) // GPS debugging, can be removed now since functional
  {
    String status = LocationService::instance().getGPSDebug();
    Log.info(status);
    if (Particle.connected())
      Particle.publish("GPSDBG", status);
  }
  else if (!first_parameter.compareTo("gpsraw")) // Can be removed
  {
    LocationService::instance().testRawGPS();
  }
  else if (!first_parameter.compareTo("battery")) // Battery debugging, can be removed now since functional
  {
    String status = "na";
    status = CityVitals::instance().getBatteryDetails();
    Log.info(status);
    if (Particle.connected())
      Particle.publish("BATT", status);
  }
  else if (!first_parameter.compareTo("lowbatt")) // Slack debugging can be removed
  {
    if (!second_parameter.compareTo("test"))
    {
      String payload = "soc=test,batt_v=0.00,reason=manual";
      Log.info(payload);
      if (Particle.connected())
        Particle.publish("LOW_BATT", payload);
    }
  }
  else if (!first_parameter.compareTo("stationary")) // Slack debugging
  {
    if (!second_parameter.compareTo("reset"))
    {
      MotionService::resetInactivityCounter();
      Log.info("stationary reset");
    }
    else if (!second_parameter.compareTo("test")) // Slack debugging
    {
      if (!CityVitals::instance().BATT_started) {
        CityVitals::instance().startBattery();
      }
      float batt_v = CityVitals::instance().getBatteryVoltage();
      int inactive_s = MotionService::getInactivityCounter();
      String payload = String::format("inactive_s=%d,batt_v=%.2f,reason=manual", inactive_s, batt_v);
      Log.info(payload);
      if (Particle.connected())
        Particle.publish("STATIONARY", payload);
    }
    else if (!second_parameter.compareTo("sleep")) // Sleep vs Hibernation? Will verify later. Hibernation can be removed if not useful. Attempted this for remote-triggered hibernation but did not work as expected, needs further investigation.
    {
      int duration = third_parameter.toInt();
      if (duration <= 0) {
        duration = 5;
      }
      if (!fourth_parameter.compareTo("seconds"))
        CitySleep::instance().stop(duration, SECONDS);
      else if (!fourth_parameter.compareTo("hours"))
        CitySleep::instance().stop(duration, HOURS);
      else
        CitySleep::instance().stop(duration, MINUTES);
    }
    else
    {
      if (!CityVitals::instance().BATT_started) {
        CityVitals::instance().startBattery();
      }
      String charging_status = CityVitals::instance().getChargingStatus();
      float batt_v = CityVitals::instance().getBatteryVoltage();
      int inactive_s = MotionService::getInactivityCounter();
      String status = String::format("inactive_s=%d,inactivity_cfg_s=%d,charging=%c,batt_v=%.2f",
                                     inactive_s,
                                     INACTIVITY_TIME,
                                     charging_status.startsWith("1") ? '1' : '0',
                                     batt_v);
      Log.info(status);
      if (Particle.connected())
        Particle.publish("STATIONARY_STATUS", status);
    }
  }
  else if (!first_parameter.compareTo("bmewd")) // BME was inconsistent sometimes, Watchdog was added to check for loops and slack notification was added. Can be removed for Brazil.
  {
    String status = CitySense::instance().getBMEWatchdogStats();
    Log.info(status);
    if (Particle.connected())
      Particle.publish("BMEWD", status);
  }
  else if (!first_parameter.compareTo("imu")) // IMU debugging, can be removed potentially
  {
    uint32_t irq_count = MotionService::getImuIrqCount();
    uint32_t irq_total = MotionService::getImuIrqTotal();
    uint32_t last_irq_ms = MotionService::getImuIrqLastMs();
    uint32_t age_ms = 0;
    if (last_irq_ms > 0) {
      age_ms = millis() - last_irq_ms;
    }
    int inactive_s = MotionService::getInactivityCounter();
    int int_state = digitalRead(INT_ACC);
    String status = String::format("irq_boot=%lu,irq_total=%lu,last_irq_ms=%lu,age_ms=%lu,inactive_s=%d,int_acc=%d",
                                   irq_count, irq_total, last_irq_ms, age_ms, inactive_s, int_state);
    Log.info(status);
    if (Particle.connected())
      Particle.publish("IMUDBG", status);
  }
  else if (!first_parameter.compareTo("wake")) // Wake command did not function as expected, needs further investigation. Remote management feature again.
  {
    if (!second_parameter.compareTo("test"))
    {
      if (!CityVitals::instance().BATT_started) {
        CityVitals::instance().startBattery();
      }
      float batt_v = CityVitals::instance().getBatteryVoltage();
      uint32_t epoch = Time.isValid() ? Time.now() : 0;
      String payload = String::format("batt_v=%.2f,reason=manual,ts=%lu", batt_v, epoch);
      Log.info(payload);
      if (Particle.connected())
        Particle.publish("WAKE", payload);
    }
  }
  else if (!first_parameter.compareTo("solar") || !first_parameter.compareTo("input")) // Solar debugging.
  {
    String status = "na";
    status = CityVitals::instance().getSolarData();
    status += "," + String((unsigned int)CityVitals::instance().getVBUSStatusCode());
    Log.info(status);
    if (Particle.connected())
      Particle.publish("SOLAR", status);
  } 
  else if (!first_parameter.compareTo("opc"))
  {
    String opcdata = CitySense::instance().last_opc_data;
    Log.info(opcdata);
    if (Particle.connected())
      Particle.publish("OPC", opcdata);
  }
  //END COMMANDS
  else if (!first_parameter.compareTo("enable3v"))
  {
    if (!second_parameter.compareTo("on"))
    {
      CS_core::instance().enable3V3(1);
    }
    else if (!second_parameter.compareTo("off"))
    {
      CS_core::instance().enable3V3(0);
    }
  }
  else if (!first_parameter.compareTo("enable5v"))
  {
    if (!second_parameter.compareTo("on"))
    {
      CS_core::instance().enable5V(1);
    }
    else if (!second_parameter.compareTo("off"))
    {
      CS_core::instance().enable5V(0);
    } 
  }
  else if (!first_parameter.compareTo("3v3on"))
  {
    Serial.println("Turning ON 3V3");
    CS_core::instance().enable3V3(true);
    delay(DTIME);
    Serial.println("Turning ON GPS");
    LocationService::instance().start();
    delay(DTIME);
    Serial.println("Turning ON Vitals");
    CityVitals::instance().init();
    delay(DTIME);
    Serial.println("Turning ON NOISE sensor");
    CitySense::instance().startNOISE();
    delay(DTIME);
    Serial.println("Turning ON Temperature sensor");
    CitySense::instance().startTEMP();
    delay(DTIME);
    Serial.println("Turning ON Gas ADC");
    CitySense::instance().startGAS();
    delay(DTIME);
  }
  else if (!first_parameter.compareTo("3v3off"))
  {
    Serial.println("Turning OFF GPS and ACC");
    MotionService::instance().stop();
    LocationService::instance().stop();
    delay(DTIME);
    Serial.println("Turning OFF Vitals");
    CityVitals::instance().stop_all();
    delay(DTIME);
    Serial.println("Turning OFF NOISE sensor");
    CitySense::instance().stopNOISE();
    delay(DTIME);
    Serial.println("Turning OFF Temperature sensor");
    CitySense::instance().stopTEMP();
    delay(DTIME);
    Serial.println("Turning OFF 3V3");
    CS_core::instance().enable3V3(false);
    delay(DTIME);
    Serial.println("Turning OFF Gas ADC");
    CitySense::instance().stopGAS();
    delay(DTIME);
  }
  else if (!first_parameter.compareTo("5von"))
  {
    Serial.println("Turning ON 5V");
    CS_core::instance().enable5V(true);
    delay(DTIME);
  }
  else if (!first_parameter.compareTo("5voff"))
  {
    Serial.println("Turning OFF OPC");
    CitySense::instance().stopOPC();
    delay(DTIME);
    Serial.println("Turning OFF 5V");
    CS_core::instance().enable5V(false);
  }
  else if (!first_parameter.compareTo("opcon")){
      CS_core::instance().enable5V(true);
      delay(1s);
      Serial.println("Turning ON OPC");
      CitySense::instance().startOPC();
      }
  else if (!first_parameter.compareTo("opcoff")){
    Serial.println("Turning OFF OPC");
      CitySense::instance().stopOPC();
  }
  return 1;
}
