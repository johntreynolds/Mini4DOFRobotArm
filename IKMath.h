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
            void processIKSerial();
            IKStatus validateAngles(const ArmAngles &a);
            
        private:
            
    };

#endif