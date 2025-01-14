#include "Particle.h"
#include "motion_service.h"

#define LOW_POWER

float   sampleRate = 6.25;  // HZ - Samples per second - 0.781, 1.563, 3.125, 6.25, 12.5, 25, 50, 100, 200, 400, 800, 1600Hz
uint8_t accelRange = 2;     // Accelerometer range = 2, 4, 8, 16g

int32_t result;

KXTJ3 myIMU(0x0E); // Address can be 0x0E or 0x0F

MotionService *MotionService::_instance = nullptr;
int MotionService::inactivity_counter = 0;


void MotionService::timer_fnc(){
   inactivity_counter++;
   Serial.print("Inactivity counter: ");Serial.println(inactivity_counter);
    pinMode(18, INPUT); // Accelerometer interrupt
}


Timer inactivity_timer(1000, MotionService::timer_fnc);

MotionService::MotionService() {
}

int MotionService::start()
{
    //Particle.variable("OVERRIDE_AS", OVVERRIDE_AUTOSLEEP);
    uint8_t errorAccumulator = 0;
    if (HW_VERSION == V4)
    {
        if( myIMU.begin(sampleRate, accelRange) != 0 )
        {
            Serial.print("Failed to initialize IMU.\n");
        }
        else
        {
            Serial.print("IMU initialized.\n");
        }

        myIMU.intConf(50, 1, 10, LOW);         // Need to adjust threshold value here (Try #123, #50) 

        uint8_t readData = 0;

        // Get the ID:
        myIMU.readRegister(&readData, KXTJ3_WHO_AM_I);
        Serial.print("Who am I? 0x");
        Serial.println(readData, HEX);
    }
    else
    {      
    
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
    //attachInterrupt(WKP, resetInactivityCounter, RISING);
    //attachInterrupt(INT_ACC, MotionService::resetInactivityCounter, RISING);
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
    if(inactivity_counter > INACTIVITY_TIME){
        Serial.println("motion service loop inactivity timer");
        resetInactivityCounter();
        if(AUTOSLEEP && !OVVERRIDE_AUTOSLEEP){
        Serial.println("It's time to get some sleep");
        delay(100);
        CitySleep::instance().stop();
        }
    }
}

void MotionService::resetInactivityCounter(){
    inactivity_counter = 0;
}

int MotionService::getInactivityCounter()
{
    return inactivity_counter;
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
