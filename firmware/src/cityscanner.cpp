#include "cityscanner.h"
#include "Particle.h"
#include "cityscanner_cli.h"

//Automatic payload publish
unsigned long lastPublishTime = 0;  // Track the last publish time
const unsigned long publishInterval = 3600000; // 1-hour interval for Bologna


Cityscanner *Cityscanner::_instance = nullptr;

bool flag_sampling = false;
void set_sample() { flag_sampling = true; }
Timer sample_timer(SAMPLE_RATE * 1000, set_sample);

bool flag_vitals = false;
void set_vitals() { flag_vitals = true; }
Timer vitals_timer(VITALS_RATE * 1000, set_vitals);

bool flag_routine = false;
void set_routine() { flag_routine = true; }
Timer routine_timer(ROUTINE_RATE * 1000, set_routine);

FuelGauge fuel;

// Timer values for smart connect
uint8_t num_min_recon = 2;                  // Number of minutes waiting for reconnection
float num_hrs_on_cellular = 1.0; //5.0 / 60.0;     // Number of hours waiting to turn on the modem

uint8_t num_min_to_turnoff_modem_pwrsave = 60;   //10 before// Max num of minutes to wait before turning off modem
uint8_t num_mins_waiting_to_turnoff_modem_pwrsave = 0;  // Number of minutes elapsed in waiting to turn off modem

float num_hrs_to_turn_on_cellular_pwrsave = 2.0; //150.0 / 60.0;     // Number of hours waiting to turn on the modem in power saving mode
uint16_t num_mins_to_turn_on_cellular_pwrsave = 120; //0;          // Max num of minutes to wait before turning on modem in power saving mode

bool msg_sent = false;
bool boot_msg_sent = false;
retained uint32_t last_low_batt_alert_epoch = 0;
retained bool pending_hibernate_alert = false;
retained uint16_t pending_hibernate_v_mv = 0;
retained uint32_t pending_hibernate_epoch = 0;

String deviceID = System.deviceID();

namespace {
  const uint32_t HIBERNATE_ALERT_MAGIC = 0x48424C54; // "HBLT"
  const int EEPROM_ADDR_HIBERNATE_ALERT = 0;
  const uint32_t WAKE_ALERT_MAGIC = 0x57414B45; // "WAKE"
  const int EEPROM_ADDR_WAKE_ALERT = 64;
  const uint32_t LOWBATT_ALERT_MAGIC = 0x4C4F4242; // "LOBB"
  const int EEPROM_ADDR_LOWBATT_ALERT = 128;

  struct HibernateAlertRecord {
    uint32_t magic;
    uint16_t batt_mv;
    uint16_t reserved;
    uint32_t epoch;
    uint32_t count;
  };

  struct WakeAlertRecord {
    uint32_t magic;
    uint16_t batt_mv;
    uint16_t reserved;
    uint32_t epoch;
    uint32_t count;
  };

  struct LowBattAlertRecord {
    uint32_t magic;
    uint16_t batt_mv;
    uint16_t soc;
    uint32_t epoch;
    uint32_t count;
  };

  bool readHibernateAlert(HibernateAlertRecord &rec) {
    EEPROM.get(EEPROM_ADDR_HIBERNATE_ALERT, rec);
    return (rec.magic == HIBERNATE_ALERT_MAGIC);
  }

  void writeHibernateAlert(uint16_t batt_mv, uint32_t epoch) {
    HibernateAlertRecord rec = {};
    if (readHibernateAlert(rec)) {
      rec.count++;
    } else {
      rec.magic = HIBERNATE_ALERT_MAGIC;
      rec.count = 1;
    }
    rec.batt_mv = batt_mv;
    rec.epoch = epoch;
    EEPROM.put(EEPROM_ADDR_HIBERNATE_ALERT, rec);
  }

  void clearHibernateAlert() {
    HibernateAlertRecord rec = {};
    EEPROM.put(EEPROM_ADDR_HIBERNATE_ALERT, rec);
  }

  bool readWakeAlert(WakeAlertRecord &rec) {
    EEPROM.get(EEPROM_ADDR_WAKE_ALERT, rec);
    return (rec.magic == WAKE_ALERT_MAGIC);
  }

  void writeWakeAlert(uint16_t batt_mv, uint32_t epoch) {
    WakeAlertRecord rec = {};
    if (readWakeAlert(rec)) {
      rec.count++;
    } else {
      rec.magic = WAKE_ALERT_MAGIC;
      rec.count = 1;
    }
    rec.batt_mv = batt_mv;
    rec.epoch = epoch;
    EEPROM.put(EEPROM_ADDR_WAKE_ALERT, rec);
  }

  void clearWakeAlert() {
    WakeAlertRecord rec = {};
    EEPROM.put(EEPROM_ADDR_WAKE_ALERT, rec);
  }

  bool readLowBattAlert(LowBattAlertRecord &rec) {
    EEPROM.get(EEPROM_ADDR_LOWBATT_ALERT, rec);
    return (rec.magic == LOWBATT_ALERT_MAGIC);
  }

  void writeLowBattAlert(uint16_t batt_mv, uint16_t soc, uint32_t epoch) {
    LowBattAlertRecord rec = {};
    if (readLowBattAlert(rec)) {
      rec.count++;
    } else {
      rec.magic = LOWBATT_ALERT_MAGIC;
      rec.count = 1;
    }
    rec.batt_mv = batt_mv;
    rec.soc = soc;
    rec.epoch = epoch;
    EEPROM.put(EEPROM_ADDR_LOWBATT_ALERT, rec);
  }

  void clearLowBattAlert() {
    LowBattAlertRecord rec = {};
    EEPROM.put(EEPROM_ADDR_LOWBATT_ALERT, rec);
  }
}

Cityscanner::Cityscanner() : core(CS_core::instance()),
                             sense(CitySense::instance()),
                             store(CityStore::instance()),
                             vitals(CityVitals::instance()),
                             motionService(MotionService::instance()),
                             locationService(LocationService::instance()),
                             cellular(CityCellular::instance())
{
}

void Cityscanner::startShippingMode()
{
}

void Cityscanner::startup()
{
  Log.info("Startup");
}

void blink(void);

int Cityscanner::init()

{
  delay(3s);
  Log.info("Init block");
  checkbattery();
  initCLI();
  Log.info("Starting Core Library");
  core.begin(HW_VERSION);
  delay(DTIME);
  //sense.startupBeep();
  Serial.println("Turning ON SD card");
  store.init();
  if (SD_FORMAT_ONSTARTUP)
  store.reInit();
  delay(DTIME);
  if (MODE == PWRSAVE)
  {
    num_hrs_on_cellular = num_hrs_to_turn_on_cellular_pwrsave;
  }
  cellular.init(num_min_recon, num_hrs_on_cellular);
  delay(DTIME);
  
  if (CELLULAR_ON_STARTUP)
  {
    Cellular.on();
    Particle.connect();
    waitFor(Particle.connected, 60000);
    if (Particle.connected())
    {
      if (!msg_sent)
      {
        char strNumFiles[5];
        itoa(store.countFilesInQueue(), strNumFiles, 10);
        
        String output = String::format("%s,%s,%s,%s,%s",  
                                    deviceID.c_str(), 
                                    LocationService::instance().getEpochTime().c_str(), //Time
                                    LocationService::instance().getGPSdata().c_str(), // GPS data
                                    strNumFiles, // Number of files in Queue folder
                                    vitals.getBatteryData().c_str());  // battery data

        Particle.publish("DAT4", output);
        msg_sent = true;
      }
    }
    if (debug_mode)
      sendWarning("I_AM_OKAY");
  }

  sample_timer.start();
  vitals_timer.start();
  routine_timer.start();

  switch (MODE)
  {
  case TEST:
    break;
  default:
    Serial.println("Turning ON 3V3");
    delay(DTIME);
    core.enable3V3(true);
    delay(10s);
    Serial.println("Turning ON 5V line");
    delay(DTIME);
    core.enable5V(true);
    delay(10s);
    sense.initI2C();
    delay(10s);
    Serial.println("Turning ON GPS");
    delay(5s);
    locationService.start();
    delay(DTIME);
    Serial.println("Turning ON Vitals");
    vitals.init(); // TBC
    delay(DTIME);
    Serial.println("Turning ON NOISE sensor");
    sense.startNOISE();
    delay(DTIME);
    Serial.println("Turning ON Temperature sensor");
    delay(500);
    for (int i = 0; i < 7; i++) {
    if (sense.startTEMP()) break;
    Serial.printlnf("TEMP init retry %d", i + 1);
    delay(2000);
    }
    delay(DTIME);
    Serial.println("Turning ON Gas sensor");
    delay(3s);
    sense.startGAS(); // TBC
    delay(DTIME);
    
    delay(5s);
    Log.info("Turning ON Accelerometer");
    delay(2s);
    motionService.start();
    
    delay(DTIME);
    Serial.println("Turning ON OPC");
    delay(500);
    for (int i = 0; i < 3; i++) {
    if (sense.startOPC()) break;
    Serial.printlnf("OPC init retry %d", i + 1);
    delay(2000);  // ✅ Longer delay for SPS30
    }
    delay(DTIME);
    break;
  }
  Log.info("end INIT");
  return 1;
}

void Cityscanner::loop()
{
  if (!boot_msg_sent && Particle.connected()) {
    if (!vitals.BATT_started) {
      vitals.startBattery();
    }
    int soc = vitals.getBatterySOC();
    float batt_v = vitals.getBatteryVoltage();
    String boot_msg = String::format("name=%s,func=cityscanner,mode=%d,batt_soc=%d,batt_v=%.2f",
                                     deviceID.c_str(),
                                     MODE,
                                     soc,
                                     batt_v);
    Particle.publish("BOOT", boot_msg);

    if (soc <= 20) {
      String payload = String::format("soc=%d,batt_v=%.2f,reason=boot", soc, batt_v);
      Particle.publish("LOW_BATT", payload);
      if (Time.isValid()) {
        last_low_batt_alert_epoch = Time.now();
      } else {
        last_low_batt_alert_epoch = 1;
      }
    }
    boot_msg_sent = true;
  }

  if (Particle.connected()) {
    HibernateAlertRecord rec = {};
    if (readHibernateAlert(rec)) {
      float hv = rec.batt_mv / 1000.0f;
      String payload = String::format("batt_v=%.2f,reason=hibernate,ts=%lu,count=%lu",
                                      hv, rec.epoch, rec.count);
      Particle.publish("LOW_BATT", payload);
      clearHibernateAlert();
      pending_hibernate_alert = false;
    } else if (pending_hibernate_alert) {
      float hv = pending_hibernate_v_mv / 1000.0f;
      String payload = String::format("batt_v=%.2f,reason=hibernate,ts=%lu", hv, pending_hibernate_epoch);
      Particle.publish("LOW_BATT", payload);
      pending_hibernate_alert = false;
    }

    WakeAlertRecord wrec = {};
    if (readWakeAlert(wrec)) {
      float hv = wrec.batt_mv / 1000.0f;
      String payload = String::format("batt_v=%.2f,reason=stop,ts=%lu,count=%lu",
                                      hv, wrec.epoch, wrec.count);
      Particle.publish("WAKE", payload);
      clearWakeAlert();
    }

    LowBattAlertRecord lrec = {};
    if (readLowBattAlert(lrec)) {
      float hv = lrec.batt_mv / 1000.0f;
      String payload = String::format("soc=%u,batt_v=%.2f,reason=offline,ts=%lu,count=%lu",
                                      lrec.soc, hv, lrec.epoch, lrec.count);
      Particle.publish("LOW_BATT", payload);
      clearLowBattAlert();
    }
  }

if (millis() - lastPublishTime >= publishInterval) {
        // Construct the response string
        String response = String(Time.now());
        response += "," + LocationService::instance().getGPSdata() + ",";
        response += data_payload;  // Use the existing `data_payload` variable

        Log.info("Publishing: " + response);

        // Publish only if connected to the cloud
        if (Particle.connected()) {
            bool publishSuccess = Particle.publish("last_payload", response);

            if (publishSuccess) {
                lastPublishTime = millis(); // Update only after successful publish
                Log.info("last_payload published successfully");
            } else {
                Log.warn("Publish failed. Retrying on the next interval.");
            }
        } else {
            Log.warn("Device not connected to the cloud.");
        }

        // Prevent rapid retries by updating the last publish time regardless
        lastPublishTime = millis();
    }


  if (flag_sampling)
  {
    flag_sampling = false;
    {

     data_payload = String::format("%s,%s,%s,%s", sense.getOPCdata(OPC_DATA_VERSION).c_str(), sense.getTEMPdata().c_str(), sense.getGASdata().c_str(), sense.getNOISEdata().c_str());  //Temp,Gas,Noise
      
    }

    switch (MODE)
    {
    case IDLE:
      Log.info("Idle Mode");
      break;
    case REALTIME:
      store.logData(BROADCAST_IMMEDIATE, Data, data_payload);
      Log.info("Real Time");
      break;
    case LOGGING:
      /* Dumping data over TCP every 50*sampling_rate seconds
      Serial.print("counter: "); Serial.println(counter++);
      if (counter % 50 == 0)
        store.dumpData(ALL_FILES);*/
      // printDebug();
      store.logData(BROADCAST_NONE, Data, data_payload);
      Log.info("Data Logging");
      // Serial.print("IR: "); Serial.println(sense.getIRdata());
      Serial.print("BATT: "); Serial.println(vitals.getBatteryData());
      break;
    case PWRSAVE:
      store.logData(BROADCAST_NONE, Data, data_payload);
      Log.info("Pwrsave");
      break;
    case TEST:
      Log.info("Test");
      // printDebug();
      // store.logData(BROADCAST_NONE, Data, data_payload);
      break;
    default:
      break;
    }
  }

  if (flag_vitals)
  {
    flag_vitals = false;

    vitals_payload = vitals.getVitalsPayload();

    // Always persist vitals to SD for battery/solar testing, independent of mode.
    store.logData(BROADCAST_NONE, Vitals, vitals_payload);
    Log.info("Vitals Logging");

    // Keep prior LOGGING-mode behavior for connectivity management.
    if (MODE == LOGGING) {
      cellular.smartconnect();    // Check if cellular is connected, if not turn off modem
    }
}


  if (flag_routine)
  {
    Serial.println("Routine operations");
    Log.info("Routine operations");
    flag_routine = false;
    checkbattery();
    //cellular.smartconnect();
    switch (MODE)
    {
    case LOGGING:
      cellular.smartconnect();
      break;

    case PWRSAVE:
      Serial.println("Power Save Mode");
      if (Cellular.isOn())
      {
        Serial.println("PWRSAVE: Cellular is on");
        if (Particle.connected())
        {
          if (!msg_sent)
          {
            char strNumFiles[5];
            itoa(store.countFilesInQueue(), strNumFiles, 10);
            String output = String::format("%s,%s,%s,%s,%s",
                                        deviceID.c_str(),
                                        LocationService::instance().getEpochTime().c_str(), // Time
                                        LocationService::instance().getGPSdata().c_str(), // GPS data
                                        strNumFiles, // Number of files in Queue folder
                                        vitals.getBatteryData().c_str());  // battery data

            Particle.publish("DAT4", output);
            msg_sent = true;
          }
          Serial.println("PWRSAVE: Particle is connected");
          num_mins_waiting_to_turnoff_modem_pwrsave++;  // Increment elapsed time in waiting
          // If enough elpsed time in waiting has passed
          if (num_mins_waiting_to_turnoff_modem_pwrsave >= num_min_to_turnoff_modem_pwrsave)
          {
            num_mins_waiting_to_turnoff_modem_pwrsave = 0;
            Particle.disconnect();
            Cellular.off();   // Turn off modem
            msg_sent = false;
          }
        }
        else
        {
          Serial.println("PWRSAVE: Particle is not connected");
          bool retVal = cellular.smartconnect();
          if (retVal)
          {
            Serial.println("PWRSAVE: Smart connect True");
            num_mins_waiting_to_turnoff_modem_pwrsave = 0;
          }
          else
          {
            Serial.println("PWRSAVE: Smart connect False");
            num_mins_waiting_to_turnoff_modem_pwrsave = 0;
            num_mins_to_turn_on_cellular_pwrsave = 0;
          }
        }
      }
      else
      {
        Serial.println("PWRSAVE: Cellular is off");
        num_mins_to_turn_on_cellular_pwrsave++;
        if (num_mins_to_turn_on_cellular_pwrsave >= round(num_hrs_to_turn_on_cellular_pwrsave * 60))
        {
          Cellular.on();
          Particle.connect();
          num_mins_to_turn_on_cellular_pwrsave = 0;
        }
      }
    
    default:
      break;
    }
  }
  
  motionService.loop();
  // Serial.print("Tap: "); Serial.println(digitalRead(WKP));
}

void Cityscanner::checkbattery()
{
  // Ensure battery charger is initialized before reading
  if (!CityVitals::instance().BATT_started) {
    CityVitals::instance().startBattery();
  }

  float battery_v = CityVitals::instance().getBatteryVoltage();
  int soc = CityVitals::instance().getBatterySOC();

  if (soc <= 20 && Particle.connected()) {
    bool should_alert = false;
    if (Time.isValid()) {
      if (last_low_batt_alert_epoch == 0 ||
          (Time.now() - last_low_batt_alert_epoch) >= LOW_BATT_ALERT_INTERVAL_SEC) {
        should_alert = true;
      }
    } else if (last_low_batt_alert_epoch == 0) {
      should_alert = true;
    }

    if (should_alert) {
      uint16_t batt_mv = (uint16_t)(battery_v * 1000.0f);
      uint32_t epoch = Time.isValid() ? Time.now() : 0;
      writeLowBattAlert(batt_mv, (uint16_t)soc, epoch);
      if (Particle.connected()) {
        String payload = String::format("soc=%d,batt_v=%.2f,reason=periodic", soc, battery_v);
        Particle.publish("LOW_BATT", payload);
        clearLowBattAlert();
      }
      if (Time.isValid()) {
        last_low_batt_alert_epoch = Time.now();
      } else {
        last_low_batt_alert_epoch = 1;
      }
    }
  } else if (soc >= 25) {
    last_low_batt_alert_epoch = 0;
  }

  // Check if the battery is low
    if (battery_v < LOW_BATTERY_THRESHOLD)
    {
        if (!LOW_BATTERY_HIBERNATE) {
            Log.warn("Low battery (%.2fV) detected, hibernate disabled for test. Continuing measurements.", battery_v);
            return;
        }

        Log.info("Low battery detected. Rechecking in 10 seconds...");
        delay(10s);
        battery_v = CityVitals::instance().getBatteryVoltage();

        // If still low, enter hibernation or check charging status
        if (battery_v < LOW_BATTERY_THRESHOLD)
        {
            String message = "LOW_BATTERY_" + String(battery_v) + "_v";
            Log.info(message);
            sendWarning(message);

            // Check charging status before hibernating
            String charging_status = CityVitals::instance().getChargingStatus();
            if (charging_status.startsWith("1")) // Charging detected
            {
                Log.info("Charging detected. Staying awake.");
                return;
            }

            // No charging detected, hibernate for 30 minutes
            Log.info("Battery low and no charging detected. Hibernating for 30 minutes.");
            pending_hibernate_alert = true;
            pending_hibernate_v_mv = (uint16_t)(battery_v * 1000.0f);
            if (Time.isValid()) {
              pending_hibernate_epoch = Time.now();
            } else {
              pending_hibernate_epoch = 0;
            }
            writeHibernateAlert(pending_hibernate_v_mv, pending_hibernate_epoch);
            if (Particle.connected()) {
              String payload = String::format("batt_v=%.2f,reason=hibernate,ts=%lu", battery_v, pending_hibernate_epoch);
              Particle.publish("LOW_BATT", payload);
              pending_hibernate_alert = false;
              clearHibernateAlert();
            }
            CitySleep::instance().hibernate(30, MINUTES);
        }
    }
}

void Cityscanner::recordWakeEvent(uint16_t batt_mv, uint32_t epoch)
{
  writeWakeAlert(batt_mv, epoch);
}



void Cityscanner::sendWarning(String warning)
{
  // String tempp = deviceID + "," + Time.now() + "," + warning;
  // Log.info("Warning: " + tempp);
  store.logData(BROADCAST_NONE, Warning, warning);
  if (Particle.connected())
  {
    Particle.publish("WAR", warning);
  }
}

void blink()
{
  Serial.println("interrupt");
}

void Cityscanner::printDebug()
{
  Serial.println("-----------------------------------------------");
  Serial.print("GPS: ");
  Serial.print(locationService.getGPSdata());
  Serial.println(" *latitude,longitude*");
  Serial.print(sense.getTEMPdata());
  Serial.println(" *temperature,humidity*");
  Serial.print("TEMP-INT: ");
  Serial.print(vitals.getTempIntData());
  Serial.println(" *temperature,humidity*");
  Serial.print("GAS: ");
  Serial.print(sense.getGASdata());
  Serial.println(" *w1,r1,w2,r2*");
  Serial.print("NOISE: ");
  Serial.print(sense.getNOISEdata());
  Serial.println(" *noise_level*");
  Serial.print("SOLAR: ");
  Serial.print(vitals.getSolarData());
  Serial.println(" *input_volt,input_current*");
  Serial.print("BATT: ");
  Serial.print(vitals.getBatteryData());
  Serial.println(" *SOC,temp,voltage,current_mA,is_charging*");
  Serial.print("BATT-CHARGER: ");
  Serial.print(vitals.getChargingStatus());
  Serial.println(" *isCharging,isCharged*");
  Serial.print("FILES ON SD CARD: ");
  Serial.println(store.countFilesInQueue());
  Serial.println("-----------------------------------------------");
  Serial.println();
}
