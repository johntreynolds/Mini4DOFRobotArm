#ifndef ServoController_h
#define ServoController_h

#include <Adafruit_PWMServoDriver.h>
#include "ServoConstants.h"

//OBJECT CREATION
extern Adafruit_PWMServoDriver pwm;
            
class ServoController
    {
        public:
            //READY SERVOS
            void begin(int freq);

            //HOMING SEQUENCE
            void homeArm();
            void homeTurret();

            //PD Controller
            void turnServo(int channel, float angle);
            void setTargetPD(int channel, float angle);
            void moveServoPD(int channel);
            void moveAllServosPD();

        private:
            //PD Controller
            float targetAngle[5];
            float virtualAngle[5]; 
            float lastError[5];
            unsigned long lastTime[5]; 
        
    };

#endif