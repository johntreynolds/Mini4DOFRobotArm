#include "ServoController.h"
#include "ServoTesting.h" 
#include "IK.h"

//SETUP ITEMS
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
bool robotStopped = false;
ServoController armController;
ServoTesting testing;
IK ik;

//=========================START OF PRODUCTION CODE FOR THE ROBOT======================================================

void setup() 
  {
    Serial.begin(9600);
    Wire.begin();
    //PRODUCTION CODE
    armController.begin(50);
    armController.homeArm();

    //TESTING
    //testing.beginTest(50);
    //testing.prepServosTest(90);

  }

void loop() 
  {
    //PRODUCTION CODE
    ik.processIKSerial(armController);
    //armController.moveAllServosPD();

    //TESTING
    //testing.runServoTravelTuning();
    //testing.scanI2C();
  }


  
