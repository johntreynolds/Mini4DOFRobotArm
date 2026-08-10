#ifndef ROBOT_WEB_SERVER_H
#define ROBOT_WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "ServoConstants.h"

class RobotWebServer
  {
    private:
      const char* _ssid;
      const char* _password;
      AsyncWebServer _server;
      AsyncWebSocket _ws;

      unsigned long _lastTelemetrySend = 0;
      const unsigned long TELEMETRY_INTERVAL_MS = 50;

      // Target Storage
      float _targetX = HOMING_XYZPHI[0];
      float _targetY = HOMING_XYZPHI[1];
      float _targetZ = HOMING_XYZPHI[2];
      float _targetPhi = HOMING_XYZPHI[3];
      bool _hasNewCommand = false;

      void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

    public:

      RobotWebServer(const char* ssid = "Robot-Arm-Control", const char* password = "robot1234");
      void beginWebServer();
      void updateWebServer();
      void sendTelemetry(float x, float y, float z, float phi);
      bool getNewTarget(float &x, float &y, float &z, float &phi);
  };

#endif