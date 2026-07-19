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

    //PRODUCTION CODE
    armController.begin(50);
    armController.homeArm();
    //armController.homeTurret();
   
    //TESTING
    //testing.beginTest(50);
    //testing.prepServosTest(90);
  }

void loop()
  {
    //PRODUCTION CODE
    armController.moveAllServosPD();

    //TESTING
    //testing.runServoTravelTuning();
    //testing.scanI2C();
  }


  
