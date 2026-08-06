#ifndef ServoController_h
#define ServoController_h

#include <Adafruit_PWMServoDriver.h>
#include "ServoConstants.h"

//OBJECT CREATION
extern Adafruit_PWMServoDriver pwm;
            
class ServoController
    {
        public:
            //PD Controller
            void turnServo(int channel, float angle);
            void setTargetPD(int channel, float angle);
            void moveServoPD(int channel);
            void moveAllServosPD();

        private:
            
        
    };

#endif