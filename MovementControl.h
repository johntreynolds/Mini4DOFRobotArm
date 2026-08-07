#ifndef MovementControl_h
#define MovementControl_h

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

#include "CoreTypes.h"
#include "ServoConstants.h"
#include "IKMath.h"
#include "PIDMath.h"

class MovementControl
    {
        public:
            //READY SERVOS AND HOME
            void begin(int freq);
            void homeArm();
            void telemetry();

            //SERVO Turning
            void turnServo(int channel, float angle); 
            void moveAllServos();

            //IK ITEMS
            bool processIKSerial();
            void ikTestBox();
            void manualIKTest();

        private:
            IKMath ik;
            PIDMath pd;

            IKInput input;
            ArmAngles angles;
    };

#endif
