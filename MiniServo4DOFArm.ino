#include "ServoController.h"
#include "ServoTesting.h" 

//SETUP ITEMS
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
bool robotStopped = false;
ServoController armController;
ServoTesting testing;

//=========================START OF PRODUCTION CODE FOR THE ROBOT======================================================

void setup() 
  {
    Serial.begin(9600);
    testing.beginTest(50);
    testing.prepServosTest(10);

  }

void loop() 
  {
    //testing.scanI2C();
    testing.runServoTravelTuning();
  }


  
