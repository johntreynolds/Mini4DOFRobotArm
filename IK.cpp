#include "IK.h"
#include "ServoConstants.h"
#include <Wire.h>

ArmAngles IK::solveFullArmHori(float x, float y, float z)
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

    Serial.println("---- IK DEBUG ----");
    Serial.print("rw="); Serial.println(rw);
    Serial.print("zw="); Serial.println(zw);
    Serial.print("d=");  Serial.println(d);

    Serial.print("alphaDeg=");  Serial.println(alphaDeg);
    Serial.print("betaDeg=");   Serial.println(betaDeg);
    Serial.print("theta1=");    Serial.println(theta1);
    Serial.print("theta2=");    Serial.println(theta2);
    Serial.print("theta3=");    Serial.println(theta3);

    Serial.print("servoTurret=");   Serial.println(thetaT);
    Serial.print("shoulderAngle="); Serial.println(theta1);
    Serial.print("elbowAngle=");    Serial.println(theta2);
    Serial.print("wristAngle=");    Serial.println(theta3);
    Serial.println("------------------");

    Serial.print("FINAL shoulder="); Serial.println(result.shoulder);
    Serial.print("FINAL elbow=");    Serial.println(result.elbow);
    Serial.print("FINAL wrist=");    Serial.println(result.wrist);

    /*
      
    */

    return result;
  }

ArmAngles IK::solveFullArmVert(float x, float y, float z)
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

    Serial.println("---- IK DEBUG ----");
    Serial.print("rw="); Serial.println(rw);
    Serial.print("zw="); Serial.println(zw);
    Serial.print("d=");  Serial.println(d);

    Serial.print("alphaDeg=");  Serial.println(alphaDeg);
    Serial.print("betaDeg=");   Serial.println(betaDeg);
    Serial.print("theta1=");    Serial.println(theta1);
    Serial.print("theta2=");    Serial.println(theta2);
    Serial.print("theta3=");    Serial.println(theta3);

    Serial.print("servoTurret=");   Serial.println(thetaT);
    Serial.print("shoulderAngle="); Serial.println(theta1);
    Serial.print("elbowAngle=");    Serial.println(theta2);
    Serial.print("wristAngle=");    Serial.println(theta3);
    Serial.println("------------------");

    Serial.print("FINAL shoulder="); Serial.println(result.shoulder);
    Serial.print("FINAL elbow=");    Serial.println(result.elbow);
    Serial.print("FINAL wrist=");    Serial.println(result.wrist);

    /*
      
    */

    return result;
  }

void IK::processIKSerial(ServoController &controller)
{
    if (!Serial.available())
        return;

    // Read full line
    String line = Serial.readStringUntil('\n');
    line.trim();

    // Show EXACTLY what Arduino received
    Serial.print("RAW LINE: '");
    Serial.print(line);
    Serial.println("'");

    // Normalize separators
    line.replace(',', ' ');
    line.replace('\t', ' ');
    line.replace('\r', ' ');
    line.trim();

    // Split into tokens
    float values[4];
    int count = 0;

    int start = 0;
    while (count < 4)
    {
        int spaceIndex = line.indexOf(' ', start);

        String token;
        if (spaceIndex == -1)
        {
            token = line.substring(start);
        }
        else
        {
            token = line.substring(start, spaceIndex);
        }

        token.trim();

        if (token.length() > 0)
        {
            values[count] = token.toFloat();
            count++;
        }

        if (spaceIndex == -1)
            break;

        start = spaceIndex + 1;
    }

    // Validate
    if (count != 4)
    {
        Serial.print("Invalid command (parsed ");
        Serial.print(count);
        Serial.println(" values)");
        return;
    }

    float x = values[0];
    float y = values[1];
    float z = values[2];

    // Confirm parsed values
    Serial.print("PARSED: ");
    Serial.print(x); Serial.print(" ");
    Serial.print(y); Serial.print(" ");
    Serial.print(z); Serial.print(" ");

    // Run IK
    //ArmAngles angles = solveFullArm(x, y, z, phi);
    ArmAngles angles = solveFullArmHori(x, y, z);

    controller.turnServo(0, angles.turret);
    controller.turnServo(1, angles.shoulder);
    controller.turnServo(2, angles.elbow);
    controller.turnServo(3, angles.wrist);

    Serial.println("Target Updated");
}
