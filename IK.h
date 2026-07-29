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
            ArmAngles solveFullArmHori(float x, float y, float z);
            ArmAngles solveFullArmVert(float x, float y, float z);
            void processIKSerial(ServoController &controller);
            

        private:
            
    };

#endif