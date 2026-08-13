#ifndef IK_h
#define IK_h

#include "CoreTypes.h"
#include "ServoConstants.h"
#include <Wire.h>
#include <Arduino.h>
#include <math.h>

class IKMath
    {
        public:
            //IK Stuff
            ArmAngles solveFullArmHori(float x, float y, float z);
            ArmAngles solveFullArmVert(float x, float y, float z);
            ArmAngles solveFullArmPhi(float x, float y, float z, float phi);
            IKInput solveFullArmDK(float thetaT, float theta1, float theta2, float theta3);
            ArmAngles manualComputeRZ(float turret, float r, float z, float phi);
            void processIKSerial();
            IKStatus validation(float x, float y, float z, const ArmAngles &a);
            
        private:
            
    };

#endif