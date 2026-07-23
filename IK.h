#ifndef IK_h
#define IK_h
#include "ServoController.h"

struct ArmAngles
    {
        float turret;
        float shoulder;
        float elbow;
        float wrist;
        float claw;
        bool isValidLocation;
    };

class IK
    {
        public:
            //IK Stuff
            ArmAngles solveFullArm(float x, float y, float z, float phi);
            ArmAngles convertOffsets(ArmAngles &angles, ServoController &controller);

        private:
            
    };

#endif