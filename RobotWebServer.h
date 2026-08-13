#ifndef ROBOT_WEB_SERVER_H
#define ROBOT_WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "ServoConstants.h"
#include "CoreTypes.h"
#include "MovementControl.h"

class MovementControl;

class RobotWebServer
  {
    public:
      enum OpMode 
        {
          OPMODE_MANUAL_XYZ     = 0,
          OPMODE_LIVE_SLIDERS   = 1,
          OPMODE_REMOTE_CONTROL = 2,
          OPMODE_ANGLE_TESTING  = 3
        };

      RobotWebServer(const char* ssid = "Mini4DOFRobotArm", const char* password = "ROBOT1234");

      // Web Server Actions
      void beginWebServer();
      void updateWebServer();

      void setController(MovementControl* controller) 
        { 
          _controller = controller; 
        }

      // INFO AND TELEMETRY
      void sendTelemetry(float x, float y, float z, float phi, float turretDeg, float shoulderDeg, float elbowDeg, float wristDeg, float clawDeg);
      void log(const String &message);
      void logf(const char *format, ...);
      void updateTelemetry();

      // State Commands
      bool getNewTarget(float &x, float &y, float &z, float &phi, float &claw);
      bool getManualAngles(float &turret, float &shoulder, float &elbow, float &wrist, float &claw);
      bool checkHomeRequest();
      bool checkOpModeChanged(OpMode &newMode);

      bool getRCInputs(RCInputs &outRC);

    private:
      AsyncWebServer _server;
      AsyncWebSocket _ws;
      const char* _ssid;
      const char* _password;
      OpMode _currentOpMode = OPMODE_MANUAL_XYZ;

      // Target Storage
      float _targetX    = 0;
      float _targetY    = 100;
      float _targetZ    = 100;
      float _targetPhi  = 90;
      float _targetClaw = 180;

      //Contol Flags
      bool _hasNewCommand       = false;
      bool _homeRequested       = false;
      bool _opModeChanged       = false;
      bool _hasNewManualCommand = false;

      ArmAngles _manualAngles;

      unsigned long _lastTelemetrySend = 0;
      const unsigned long TELEMETRY_INTERVAL_MS = 50;
      uint16_t _port;

      void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

      MovementControl* _controller = nullptr;
      RCInputs _rcInputs{};
      bool _hasNewRC = false;

      unsigned long _lastTelemetry = 0;
  };

#endif