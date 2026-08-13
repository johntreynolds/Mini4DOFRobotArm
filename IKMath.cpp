#include "IKMath.h"

ArmAngles IKMath::solveFullArmHori(float x, float y, float z)
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
    float wristDelta = 180.0f - betaDeg - gammaDeg;
    float wristChi = 90.0f - alphaDeg;
    float theta3 = wristDelta + wristChi;

    result.turret = thetaT;
    result.shoulder = theta1;
    result.elbow = theta2;
    result.wrist = theta3;

    result.rw = rw;
    result.zw = zw;

    result.hasPhi = false;

    result.status = IK_OK;
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
    float wristDelta = 180.0f - betaDeg - gammaDeg;
    float wristChi = 90.0f - alphaDeg;
    float theta3 = wristDelta + wristChi - 90.0f;

    result.rw = rw;
    result.zw = zw;

    result.turret = constrain(thetaT, MIN_ANGLE[0], MAX_ANGLE[0]);
    result.elbow = constrain(theta2, MIN_ANGLE[2], MAX_ANGLE[2]);
    result.shoulder = constrain(theta1, MIN_ANGLE[1], MAX_ANGLE[1]);
    result.wrist = constrain(theta3, MIN_ANGLE[3], MAX_ANGLE[3]);

    /*
      
    */

    result.hasPhi = false;
    result.status = IK_OK;
    return result;
  }

ArmAngles IKMath::solveFullArmPhi(float x, float y, float z, float phi)
  {
    //Return value
    ArmAngles result;

    //Solve for Turret
    float thetaT = atan2(y, x) * RAD_TO_DEG;

    //Isolate Wrist Pivot (rw, zw)
    float phiRad = phi*DEG_TO_RAD;
    float rTarget = sqrt(x*x + y*y);
    float rw = rTarget - (L3*sin(phiRad));
    float zw = z + (L3*cos(phiRad));  
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
    float elbowOffset = fabs(elbowColinear - 90.0f);
    float theta2 = 270.0f - gammaDeg - elbowOffset;

    //Solve for Shoulder (theta 1)
    float alphaDeg = atan2(zw, rw) * RAD_TO_DEG;
    float cosBeta = (L1*L1 + d*d - L2*L2) / (2.0f * L1 * d);
    float betaDeg = acos(constrain(cosBeta, -1.0f, 1.0f)) * RAD_TO_DEG;
    float shoulderOffset = fabs(shoulderColinear - 90.0f);
    float theta1 = alphaDeg + betaDeg + shoulderOffset;
    
    //Solve for Wrist
    //float wristOffset = abs(wristColinear - 90.0f); NOT NEEDED RIGHT NOW
    float deltaDeg = 180.0f - gammaDeg - betaDeg;
    float chiDeg = 90.0f - alphaDeg;
    float theta3 = deltaDeg + chiDeg + phi - 90.0f;

    result.turret = thetaT;
    result.shoulder = theta1;
    result.elbow = theta2;
    result.wrist = theta3;

    result.rw = rw;
    result.zw = zw;

    result.phi = phi;
    result.hasPhi = true;

    result.status = IK_OK;
    return result;

    /*
      
    */
 
  }

ArmAngles IKMath::manualComputeRZ(float turret, float r, float z, float phi)
  {
    // Solve 2D reach using R directly as X (with Y = 0)
    ArmAngles targetAngles = solveFullArmPhi(r, 0.0f, z, phi);

    // Override turret directly from input theta
    targetAngles.turret = turret;

    return targetAngles;  
  }

IKStatus IKMath::validateAngles(const ArmAngles &a)
  {
    if (a.status == IK_TOO_FAR)
      {
        return IK_TOO_FAR;
      }
    if (a.status == IK_TOO_CLOSE)
      {
        return IK_TOO_CLOSE;
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

// ------------------------------------- DIRECT KINEMATICS ---------------------------------

IKInput IKMath::solveFullArmDK(float thetaT, float theta1, float theta2, float theta3)
  {
    IKInput dk;

    // Variable Setup
    float shoulderOffset = abs(shoulderColinear - 90.0f);
    float elbowOffset = abs(elbowColinear - 90.0f);
    float alphaBeta = theta1 - shoulderOffset;
    float gamma = 270.0f - elbowOffset - theta2;
    float gammaMini = alphaBeta + gamma - 180.0f;

    // Piece-by-piece math
    float ze = sin(alphaBeta * DEG_TO_RAD) * L1;
    float re = cos(alphaBeta * DEG_TO_RAD) * L1;
    float zw = ze + sin(gammaMini * DEG_TO_RAD) * L2;
    float rw = re + cos(gammaMini * DEG_TO_RAD) * L2;

    // Mid Variables
    float alpha = atan2(zw, rw) * RAD_TO_DEG;
    float beta = alphaBeta - alpha;
    float chi = 90.0f - alpha;
    float delta = 180.0f - gamma - beta;
    float phi = theta3 - delta - chi + 90.0f;

    float z = zw - cos(phi * DEG_TO_RAD) * L3;
    float r = rw + sin(phi * DEG_TO_RAD) *L3;

    float x = r * cos(thetaT * DEG_TO_RAD);
    float y = r * sin(thetaT * DEG_TO_RAD);

    dk.x = x;
    dk.y = y;
    dk.z = z;
    dk.phi = phi;
    dk.hasPhi = true;

    return dk;

  }
