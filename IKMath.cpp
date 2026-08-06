#include "IKMath.h"
#include "ServoConstants.h"
#include "CoreTypes.h"

#include <Wire.h>
#include <Arduino.h>
#include <math.h>

ArmAngles IKMath::solveFullArmHori(float x, float y, float z) //DOESN'T DEAL WITH ISSUES
  {
    //Return value
    ArmAngles result;

    //Solve for Turret
    float thetaT = atan2(y, x) * RAD_TO_DEG;

    //Isolate Wrist Pivot (rw, zw)
    float rTarget = sqrt(x*x + y*y);
    float rw = rTarget - L3;
    float zw = z;  
    float d = sqrt(rw*rw + zw*zw);

    if (d > L1 + L2)
      {
        result.status = IK_TOO_FAR;
        return result;
      }
    if (d < fabs(L1 - L2))
      {
        result.status = IK_TOO_CLOSE;
        return result;
      }

    //Solve for Elbow (theta 2)
    float cosGamma = (L1*L1 + L2*L2 - d*d) / (2.0f * L1 * L2);
    float gammaDeg = acos(constrain(cosGamma, -1.0f, 1.0f)) * RAD_TO_DEG; 
    float elbowOffset = abs(elbowColinear - 90.0f);
    float theta2 = 270.0f - gammaDeg - elbowOffset;

    //Solve for Shoulder (theta 1)
    float alphaDeg = atan2(zw, rw) * RAD_TO_DEG;
    float cosBeta = (L1*L1 + d*d - L2*L2) / (2.0f * L1 * d);
    float betaDeg = acos(constrain(cosBeta, -1.0f, 1.0f)) * RAD_TO_DEG;
    float shoulderOffset = abs(shoulderColinear - 90.0f);
    float theta1 = alphaDeg + betaDeg + shoulderOffset;
    
    //Solve for Wrist
    float wristOffset = abs(wristColinear - 90.0f);
    float theta3 = (180 - betaDeg - gammaDeg) + (90.0f - alphaDeg);

    result.turret = thetaT;
    result.shoulder = theta1;
    result.elbow = theta2;
    result.wrist = theta3;

    return result;

    /*
      
    */
 
  }

ArmAngles IKMath::solveFullArmVert(float x, float y, float z)
  {
    //Return value
    ArmAngles result;

    //Solve for Turret
    float thetaT = atan2(y, x) * RAD_TO_DEG;

    //Isolate Wrist Pivot (rw, zw)
    float rTarget = sqrt(x*x + y*y);
    float rw = rTarget;
    float zw = z + L3;  
    float d = sqrt(rw*rw + zw*zw);

    //Solve for Elbow (theta 2)
    float cosGamma = (L1*L1 + L2*L2 - d*d) / (2.0f * L1 * L2);
    float gammaDeg = acos(constrain(cosGamma, -1.0f, 1.0f)) * RAD_TO_DEG; 
    float elbowOffset = abs(elbowColinear - 90.0f);
    float theta2 = 270.0f - gammaDeg - elbowOffset;

    //Solve for Shoulder (theta 1)
    float alphaDeg = atan2(zw, rw) * RAD_TO_DEG;
    float cosBeta = (L1*L1 + d*d - L2*L2) / (2.0f * L1 * d);
    float betaDeg = acos(constrain(cosBeta, -1.0f, 1.0f)) * RAD_TO_DEG;
    float shoulderOffset = abs(shoulderOffset - 90.0f);
    float theta1 = alphaDeg + betaDeg + shoulderOffset;
    
    //Solve for Wrist
    float wristOffset = abs(shoulderColinear - 90.0f);
    float theta3 = (180 - betaDeg - gammaDeg) + (90.0f - alphaDeg);

    result.turret = constrain(thetaT, MIN_ANGLE[0], MAX_ANGLE[0]);
    result.elbow = constrain(theta2, MIN_ANGLE[2], MAX_ANGLE[2]);
    result.shoulder = constrain(theta1, MIN_ANGLE[1], MAX_ANGLE[1]);
    result.wrist = constrain(theta3, MIN_ANGLE[3], MAX_ANGLE[3]);

    /*
      
    */

    return result;
  }

IKStatus IKMath::validateAngles(const ArmAngles &a)
  {
    if (a.status != IK_OK)
      {
        return IK_UNREACHABLE;
      }
    if (a.turret < MIN_ANGLE[0] || a.turret > MAX_ANGLE[0])
      {
        return IK_TURRET_LIMIT;
      } 
    if (a.shoulder < MIN_ANGLE[1] || a.shoulder > MAX_ANGLE[1])
      {
        return IK_SHOULDER_LIMIT;
      } 
    if (a.elbow < MIN_ANGLE[2] || a.elbow > MAX_ANGLE[2])
      {
        return IK_ELBOW_LIMIT;
      } 
    if (a.wrist < MIN_ANGLE[3] || a.wrist > MAX_ANGLE[3])
      {
        return IK_WRIST_LIMIT;
      } 

    return IK_OK;   
  }
