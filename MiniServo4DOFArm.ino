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
    controller.homeArm();

    //Web Server Begin
    webServer.beginWebServer();
  }

void loop()
  {
    // WEB SERVER UPDATES
    webServer.updateWebServer();

    // CHECK E STOP
    if (webServer.isEStopActive())
      {
        if (!controller.isEStopped())
          {
            controller.emergencyStop();
          }
        return;
        
      }
    else
      {
        if (controller.isEStopped())
          {
            controller.releaseEStop();
          }
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

    float targetX, targetY, targetZ, targetPhi, targetClaw;
    if (webServer.getNewTarget(targetX, targetY, targetZ, targetPhi, targetClaw)) 
      {
        switch (currentMode) 
          {
            case RobotWebServer::OPMODE_MANUAL_XYZ:
              controller.runManualXYZ(targetX, targetY, targetZ, targetPhi, targetClaw);
              break;
            case RobotWebServer::OPMODE_LIVE_SLIDERS:
              controller.runLiveSliders(targetX, targetY, targetZ, targetPhi, targetClaw);
              break;
            case RobotWebServer::OPMODE_REMOTE_CONTROL:
              controller.remoteControl();
              break;
          }
      }

    // Send Telemetry to Board
    // SEND TELEMETRY TO WEB CLIENT
    float curX, curY, curZ, curPhi;
    float t, s, e, w, c;
    controller.getCurrentPose(curX, curY, curZ, curPhi);
    controller.getCurrentAngles(t, s, e, w, c);

    webServer.sendTelemetry(curX, curY, curZ, curPhi, t, s, e, w, c);
  }
