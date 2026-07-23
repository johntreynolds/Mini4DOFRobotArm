#include "IK.h"
#include "ServoConstants.h"
#include <Wire.h>

ArmAngles IK::solveFullArm(float x, float y, float z, float phi)
  {
    ArmAngles result;
    float turretDegrees = constrain(atan2(y, x) * (180.0/M_PI), MIN_ANGLE[0], MAX_ANGLE[0]);
    result.turret = turretDegrees;
  }

  ArmAngle convertOffsets(ArmAngles &angles, ServoController &controller);
    {
      ArmAngles result;
    }