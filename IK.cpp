#include "IK.h"
#include "ServoConstants.h"
#include <Wire.h>

ArmAngles IK::solveFullArm(float x, float y, float z, float phi)
  {
    //Return value
    ArmAngles result;

    //Solve for Turret
    float turretDegrees = atan2(y, x) * RAD_TO_DEG;

    //Isolate Wrist Pivot (rw, zw)
    float phiToRadians = phi * DEG_TO_RAD;
    float rTarget = x;   // forward distance only
    float rw = rTarget - (L3 * cos(phiToRadians));
    float zw = z - (L3 * sin(phiToRadians));  
    float d = sqrt(rw*rw + zw*zw);

    //Solve for Elbow (theta 2)
    float cosGamma = (L1*L1 + L2*L2 - d*d) / (2.0f * L1 * L2);
    float innerGamma = acos(constrain(cosGamma, -1.0f, 1.0f)) * RAD_TO_DEG; 
    float theta2 = 180.0f - innerGamma;

    //Solve for Shoulder (theta 1)
    float alphaDeg = atan2(zw, rw) * RAD_TO_DEG;
    float cosBeta = (L1*L1 + d*d - L2*L2) / (2.0f * L1 * d);
    float betaDeg = acos(constrain(cosBeta, -1.0f, 1.0f)) * RAD_TO_DEG;
    float theta1 = betaDeg + alphaDeg;
    
    //Solve for Wrist
    float theta3 = theta1 - theta2 - phi;
    
    //RESULTS
    float servoTurret = 90.0f + turretDegrees;
    float elbowAngle = elbowColinear - (theta2 - 90.0f);
    float shoulderAngle = shoulderColinear + (theta1 - 90.0f);
    float wristAngle = wristColinear + (theta3 - 90.0f);

    result.turret = constrain(servoTurret, MIN_ANGLE[0], MAX_ANGLE[0]);
    result.elbow = constrain(elbowAngle, MIN_ANGLE[2], MAX_ANGLE[2]);
    result.shoulder = constrain(shoulderAngle, MIN_ANGLE[1], MAX_ANGLE[1]);
    result.wrist = constrain(wristAngle, MIN_ANGLE[3], MAX_ANGLE[3]);

    return result;
  }

void IK::processIKSerial(ServoController &controller)
{
    if (Serial.available())
    {
        float x = Serial.parseFloat();
        float y = Serial.parseFloat();
        float z = Serial.parseFloat();
        float phi = Serial.parseFloat();

        // Ignore empty commands
        if (x == 0 && y == 0 && z == 0 && phi == 0)
        {
            while(Serial.available())
            {
                Serial.read();
            }
            return;
        }

        ArmAngles angles = solveFullArm(x, y, z, phi);

        controller.turnServo(0, angles.turret);
        controller.turnServo(1, angles.shoulder);
        controller.turnServo(2, angles.elbow);
        controller.turnServo(3, angles.wrist);


        Serial.println("Target Updated");

        Serial.print("x=");
        Serial.println(x);

        Serial.print("y=");
        Serial.println(y);

        Serial.print("z=");
        Serial.println(z);

        Serial.print("phi=");
        Serial.println(phi);
    }
}
