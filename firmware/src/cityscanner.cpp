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

String deviceID = System.deviceID();

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
    delay(10s);
    core.enable3V3(true);
    delay(DTIME);
    Serial.println("Turning ON 5V line");
    delay(10s);
    core.enable5V(true);
    delay(DTIME);
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
    delay(10s);
    sense.startTEMP(); // TBC
    delay(DTIME);
    Serial.println("Turning ON Gas sensor");
    delay(3s);
    sense.startGAS(); // TBC
    delay(DTIME);
  
    Log.info("Turning ON Accelerometer");
    delay(5s);
    motionService.start();
    delay(DTIME);
      Serial.println("Turning ON OPC");
      delay(5s);
      sense.startOPC();
      delay(DTIME);
    
    break;
  }
  Log.info("end INIT");
  return 1;
}

void Cityscanner::loop()
{
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

    vitals_payload = String::format("%s,%s,%s,%s,%s", vitals.getBatteryData().c_str(), // SOC,temp,voltage,voltage_Partice,current_mA,is_charging
                                    vitals.getChargingStatus().c_str(),                // isCharging,isCharged
                                    vitals.getTempIntData().c_str(),                   // temp_int,hum_int
                                    vitals.getSolarData().c_str(),                     // solar_volt,solar_current
                                    vitals.getSignalStrenght().c_str());               // Cellular signal strenght

    switch (MODE)
    {
      case LOGGING:
      store.logData(BROADCAST_NONE, Vitals, vitals_payload);
      Log.info("Vitals Logging");
      cellular.smartconnect();    // Check if cellular is connected, if not turn off modem
      break;
    case PWRSAVE:
      store.logData(BROADCAST_NONE, Vitals, vitals_payload);
      Log.info("Vitals Logging");
      break;
    default:
      break;
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
  int batt_volt_adc = analogRead(BATTERY_VOLTAGE_PIN);
  float battery_v = (batt_volt_adc / 4095.0) * 3.3 * 2;
  Log.info("Battery voltage:" + String(battery_v));
  // Check if the battery is low
    if (battery_v < LOW_BATTERY_THRESHOLD)
    {
        Log.info("Low battery detected. Rechecking in 10 seconds...");
        delay(10s);
        batt_volt_adc = analogRead(BATTERY_VOLTAGE_PIN);
        battery_v = (batt_volt_adc / 4095.0) * 3.3 * 2;

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
            CitySleep::instance().hibernate(30, MINUTES);
        }
    }
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
  Serial.println(" *solar_volt,solar_current*");
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