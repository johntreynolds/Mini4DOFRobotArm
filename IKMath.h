#ifndef IK_h
#define IK_h

#include "CoreTypes.h"

class IKMath
    {
        public:
            //IK Stuff
            ArmAngles solveFullArmHori(float x, float y, float z);
            ArmAngles solveFullArmVert(float x, float y, float z);
            void processIKSerial();
            IKStatus validateAngles(const ArmAngles &a);
            
        private:
            
    };

#endif