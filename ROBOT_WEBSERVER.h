#ifndef ROBOT_WEBSERVER_H
#define ROBOT_WEBSERVER_H

#include <Arduino.h>

// Core Web Server Interface Functions
void initWebServer(const char* ssid, const char* password);
void updateWebServer();
void sendDataToComputer(String message);

// Internal handler for parsed incoming socket payloads
void processIncomingCommand(String command);

#endif