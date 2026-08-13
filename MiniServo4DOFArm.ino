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
            static unsigned long lastTime = 0;
            unsigned long now = millis();
            if (lastTime == 0) 
              {
                lastTime = now;
              }
            float deltaTime = (now - lastTime) / 1000.0f;
            lastTime = now;

            if (deltaTime > 0.05f) 
              {
                deltaTime = 0.05f;
              }

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

    static unsigned long lastServoUpdate = 0;
    unsigned long currentMillis = millis();

    // Step PD controller and update physical PCA9685 servos at steady 50 Hz
    if (currentMillis - lastServoUpdate >= 20) 
      {
        lastServoUpdate = currentMillis;

        // Drive PD virtual angles to physical outputs (except raw manual angle testing)
        if (currentMode != RobotWebServer::OPMODE_ANGLE_TESTING)
          {
            controller.moveAllServos();
          }
      }
    
    
    // Send Telemetry to Board
    // SEND TELEMETRY TO WEB CLIENT
    static unsigned long lastTelemetry = 0;
    if (currentMillis - lastTelemetry >= 50) 
      {
        lastTelemetry = currentMillis;

        float curX, curY, curZ, curPhi;
        float t, s, e, w, c;
        controller.getCurrentPose(curX, curY, curZ, curPhi);
        controller.getCurrentAngles(t, s, e, w, c);

        webServer.sendTelemetry(curX, curY, curZ, curPhi, t, s, e, w, c);
      }
  }
