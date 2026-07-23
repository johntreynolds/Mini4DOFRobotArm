#include "IK.h"
#include "ServoConstants.h"
#include <Wire.h>

ArmAngles IK::solveFullArm(float x, float y, float z, float phi)
  {
    ArmAngles result;
    float turretDegrees = atan2(y, x) * (180.0 / M_PI);
    if (turretDegrees < 0)
      {
        turretDegrees += 360;
      }
    turretDegrees = constrain(turretDegrees, MIN_ANGLE[0], MAX_ANGLE[0]);
    result.turret = turretDegrees;
    return result;
  }

  ArmAngles IK::convertOffsets(ArmAngles &angles, ServoController &controller)
    {
      ArmAngles result;
      return result;
    }