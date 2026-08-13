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

// 1. WebServer Instance (AP Credentials)
RobotWebServer webServer("RobotArmMini4DOF", "Robot1234");

// Track current operation state
RobotWebServer::OpMode currentMode = RobotWebServer::OpMode::OPMODE_MANUAL_XYZ;

//=========================START OF PRODUCTION CODE FOR THE ROBOT======================================================

void setup() 
  {
    //PRODUCTION CODE

    //Robot Arm Begin
    controller.begin(50);
    Wire.setClock(400000);
    controller.homeArm();

    // E STOP CONTROLLER
    webServer.setController(&controller);

    //Web Server Begin
    webServer.beginWebServer();
  }

void loop()
  {
    // WEB SERVER UPDATES
    webServer.updateWebServer();
    webServer.updateTelemetry();

    if (controller.isEStopped()) 
      {
        // Arm is E-Stopped; do not execute motion or IK updates
        return; 
      }

    // Check for OpMode Changes
    RobotWebServer::OpMode newMode;
    if (webServer.checkOpModeChanged(newMode))
      {
        currentMode = newMode;
        controller.setOpMode(static_cast<uint8_t>(currentMode));
      }

    // Check for Homing
    if (webServer.checkHomeRequest())
      {
        controller.homeArm();
      }

    // Process OpModes and Motion Commands
    
    switch (currentMode) 
      {
        case RobotWebServer::OPMODE_MANUAL_XYZ: 
          {
            float targetX, targetY, targetZ, targetPhi, targetClaw;
            if (webServer.getNewTarget(targetX, targetY, targetZ, targetPhi, targetClaw)) 
              {
                controller.runManualXYZ(targetX, targetY, targetZ, targetPhi, targetClaw);
              }
            break;
          }

        case RobotWebServer::OPMODE_LIVE_SLIDERS: 
          {
            float targetX, targetY, targetZ, targetPhi, targetClaw;
            if (webServer.getNewTarget(targetX, targetY, targetZ, targetPhi, targetClaw)) 
              {
                controller.runLiveSliders(targetX, targetY, targetZ, targetPhi, targetClaw);
              }
            break;
          }

        case RobotWebServer::OPMODE_REMOTE_CONTROL: 
          {
            RCInputs rc;
            if (webServer.getRCInputs(rc))
              {
                controller.handleRCCommand(rc);
              }
            break;
          }

        case RobotWebServer::OPMODE_ANGLE_TESTING: 
          {
            float t, s, e, w, c;
            // Check manual angles directly!
            if (webServer.getManualAngles(t, s, e, w, c)) 
              {
                controller.manualAngleMove(t, s, e, w, c);
              }
            break;
          }
      }

    controller.timingServoUpdate();
  }
