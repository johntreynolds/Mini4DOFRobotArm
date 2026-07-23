#ifndef IK_h
#define IK_h

struct ArmAngles
    {
        float turret;
        float shoulder;
        float elbow;
        float wrist;
        float claw;
        bool isValidLocation;
    }

class IV
    {
        public:
            //IK Stuff
            ArmAngles solveFullArm(float x, float y, float z, float phi);
            ArmAngle convertOffsets(ArmAngles &angles, ServoController &controller);

        private:

            
    };

#endif