#ifndef PIDMath_h
#define PIDMath_h

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "ServoConstants.h"

//OBJECT CREATION
extern Adafruit_PWMServoDriver pwm;
            
class PIDMath
    {
        public:
            //PD Controller
            void resetPDToHome();
            void setTargetPD(int channel, float angle);
            void pdServoMath(int channel);
            float getVirtualAngle(int channel);

        private:
            //Variable Arrays
            float targetAngle[5]      = {90.0f, 180.0f, 180.0f, 160.0f, 180.0f};
            float virtualAngle[5]     = {90.0f, 180.0f, 180.0f, 160.0f, 180.0f};
            float lastError[5]        = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
            unsigned long lastTime[5] = {0, 0, 0, 0, 0};
            
    };

#endif