//ESP32 CODEBASE

#include "MovementControl.h"
#include "IKMath.h"
#include "PIDMath.h"
#include "ServoTesting.h"
#include <Wire.h>

//SETUP ITEMS
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
ServoTesting testing;
MovementControl controller;

//=========================START OF PRODUCTION CODE FOR THE ROBOT======================================================

void setup() 
  {
    //PRODUCTION CODE
    controller.begin(50);
    controller.homeArm();
    Serial.println("ROBOT READY");
    delay(1000);
  }

void loop()
  {
    //PRODUCTION CODE
    //controller.ikTestBox();
    controller.manualIKTest();

    //TESTING
    //testing.runServoTravelTuning();
    //testing.scanI2C();
  }
