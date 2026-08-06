#include "PIDMath.h"
#include "ServoTesting.h" 
#include "IKMath.h"

//SETUP ITEMS
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
bool robotStopped = false;
ServoTesting testing;
MovementControl controller;

//=========================START OF PRODUCTION CODE FOR THE ROBOT======================================================

void setup() 
  {
    Serial.begin(9600);
    Wire.begin();
    //PRODUCTION CODE
    controller.begin(50);
    controller.homeArm();

    //TESTING
    //testing.beginTest(50);
    //testing.prepServosTest(90);

  }

void loop() 
  {
    //PRODUCTION CODE
    //ik.processIKSerial(armController);
    //armController.moveAllServosPD();

    //TESTING
    
    for (int i = 0; i < 200; i++)
      {
        ArmAngles t;

        // ---- Pose 1 ----
        t = ik.solveFullArmHori(100, 0, 140);
        armController.setTargetPD(0, t.turret);
        armController.setTargetPD(1, t.shoulder);
        armController.setTargetPD(2, t.elbow);
        armController.setTargetPD(3, t.wrist);

        for (int i = 0; i < 300; i++)
          {
            armController.moveAllServosPD();
            delay(5);   // allow PD to stabilize
          }

        // ---- Pose 2 ----
        t = ik.solveFullArmHori(100, 0, 100);
        armController.setTargetPD(0, t.turret);
        armController.setTargetPD(1, t.shoulder);
        armController.setTargetPD(2, t.elbow);
        armController.setTargetPD(3, t.wrist);

        for (int i = 0; i < 300; i++)
          {
            armController.moveAllServosPD();
            delay(5);
          }

        // ---- Pose 3 ----
        t = ik.solveFullArmHori(140, 0, 100);
        armController.setTargetPD(0, t.turret);
        armController.setTargetPD(1, t.shoulder);
        armController.setTargetPD(2, t.elbow);
        armController.setTargetPD(3, t.wrist);

        for (int i = 0; i < 300; i++)
          {
            armController.moveAllServosPD();
            delay(5);
          }

        // ---- Pose 4 ----
        t = ik.solveFullArmHori(140, 0, 140);
        armController.setTargetPD(0, t.turret);
        armController.setTargetPD(1, t.shoulder);
        armController.setTargetPD(2, t.elbow);
        armController.setTargetPD(3, t.wrist);

        for (int i = 0; i < 300; i++)
          {
            armController.moveAllServosPD();
            delay(5);
          }
    }

    

    //testing.runServoTravelTuning();
    //testing.scanI2C();
  }


  
