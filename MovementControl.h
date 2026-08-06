#ifndef MovementControl_h
#define MovementControl_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "ServoConstants.h"
#include "CoreTypes.h"

//OBJECT CREATION
extern Adafruit_PWMServoDriver pwm;

class MovementControl
    {
        public:
            //READY SERVOS
            void begin(int freq);

            //Basic Servo Turn
            void turnServo(int channel, float angle);

            //HOMING SEQUENCE
            void homeArm();

            //Take Serial Input
            void processIKSerial();


        private:
    };

#endif
