#ifndef ROBOT_WEB_SERVER_H
#define ROBOT_WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "ServoConstants.h"

// OpMode Definitions

class RobotWebServer
  {
    public:
      enum OpMode 
        {
          OPMODE_MANUAL_XYZ = 0,
          OPMODE_LIVE_SLIDERS = 1,
          OPMODE_REMOTE_CONTROL = 2
        };
      RobotWebServer(const char* ssid = "Mini4DOFRobotArm", const char* password = "ROBOT1234");

      // Web Server Actions
      void beginWebServer();
      void updateWebServer();

      // INFO AND TELEMETRY
      void sendTelemetry(float x, float y, float z, float phi, float turretDeg, float shoulderDeg, float elbowDeg, float wristDeg, float clawDeg);
      void log(const String &message);
      void logf(const char *format, ...);

      // State Commands
      bool getNewTarget(float &x, float &y, float &z, float &phi, float &claw);
      bool isEStopActive();
      bool checkHomeRequest();
      bool checkOpModeChanged(OpMode &newMode);

      

    private:
      AsyncWebServer _server;
      AsyncWebSocket _ws;
      const char* _ssid;
      const char* _password;

      unsigned long _lastTelemetrySend = 0;
      const unsigned long TELEMETRY_INTERVAL_MS = 100;
      uint16_t _port;

      // Target Storage
      float _targetX;
      float _targetY;
      float _targetZ;
      float _targetPhi;
      float _targetClaw;

      //Contol Flags
      bool _hasNewCommand = false;
      bool _eStopActive = false;
      bool _homeRequested = false;
      bool _opModeChanged = false;

      OpMode _currentOpMode = OPMODE_MANUAL_XYZ;

      void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
  };

#endif