#ifndef MovementControl_h
#define MovementControl_h

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

#include "CoreTypes.h"
#include "ServoConstants.h"
#include "IKMath.h"
#include "PIDMath.h"
#include "RobotWebServer.h"

class MovementControl
    {
        public:
            void begin(int freq);

            // CORE ACTION METHODS
            void homeArm();
            void emergencyStop();
            void releaseEStop();
            void setOpMode(uint8_t mode);

            // GETTERS
            void getCurrentPose(float &x, float &y, float &z, float &phi);
            void getCurrentAngles(float &turret, float &shoulder, float &elbow, float &wrist, float &claw);

            bool isEStopped() const
                {
                    return _isEStopped;
                }
            uint8_t getOpMode() const
                {
                    return _currentOpMode;
                }
            
            //SERVO TURNING
            void turnServo(int channel, float angle); 
            void moveAllServos();

            //IK TESTING
            //bool processIKSerial();
            //void ikTestBox();
            //void manualIKTest();
            //void telemetry();

            // TO BE COMPLETED
            //bool processWebTarget(float x, float y, float z, float phi);
            bool rotateAroundXYZ(float x, float y, float z, float phiMin, float phiMax);

            // OPMODE HANDLING
            bool manualCompute(float x, float y, float z, float phi);
            bool runManualXYZ(float x, float y, float z, float phi, float clawPercent);
            bool runLiveSliders(float x, float y, float z, float phi, float clawPercent);
            void remoteControl(); //TO BE COMPLETED LATER

            // CLAW CONTROl
            void moveClaw(float percent);

            IKInput getInput() const 
                { 
                    return input; 
                }

        private:

            IKInput input;
            ArmAngles angles;

            IKMath ik;
            PIDMath pd;

            bool _isEStopped = false;
            uint8_t _currentOpMode = 0;

    };

#endif
