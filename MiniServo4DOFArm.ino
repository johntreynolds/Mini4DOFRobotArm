//ESP32 CODEBASE

#include "MovementControl.h"
#include "IKMath.h"
#include "PIDMath.h"
#include "ServoTesting.h"
#include "RobotWebServer.h"
#include <Wire.h>

//SETUP ITEMS
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
ServoTesting testing;
MovementControl controller;
RobotWebServer webServer;

//=========================START OF PRODUCTION CODE FOR THE ROBOT======================================================

void setup() 
  {
    //PRODUCTION CODE

    //Robot Arm Begin
    controller.begin(50);
    controller.homeArm();

    //Web Server Begin
    webServer.beginWebServer();

    delay(1000);
  }

void loop()
  {
    //ROBOT WEB INTERFACING
    webServer.updateWebServer();

    float targetX, targetY, targetZ, targetPhi;
    if (webServer.getNewTarget(targetX, targetY, targetZ, targetPhi)) 
      {
        controller.processWebTarget(targetX, targetY, targetZ, targetPhi);
      }

    //MOVES SERVOS
    controller.moveAllServos();

    IKInput target = controller.getInput();
    webServer.sendTelemetry(target.x, target.y, target.z, target.phi);
    delay(5);
 
    //TESTING
    //testing.runServoTravelTuning();
    //testing.scanI2C();
    //controller.ikTestBox();
    //controller.manualIKTest();
  }
