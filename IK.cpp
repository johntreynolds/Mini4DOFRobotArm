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
    float rTarget = sqrt(x*x + y*y);
    float rw = rTarget - (L3 * cos(phi * DEG_TO_RAD));
    float zw = z - (L3 * sin(phi * DEG_TO_RAD));  
    float d = sqrt(rw*rw + zw*zw);

    //Solve for Elbow (theta 2)
    float cosGamma = (L1*L1 + L2*L2 - d*d) / (2.0f * L1 * L2);
    float innerGamma = acos(constrain(cosGamma, -1.0f, 1.0f)) * RAD_TO_DEG; 
    float theta2 = 180.0f - innerGamma;

    //Solve for Shoulder (theta 1)
    float alphaDeg = atan2(zw, rw) * RAD_TO_DEG;
    float cosBeta = (L1*L1 + d*d - L2*L2) / (2.0f * L1 * d);
    float betaDeg = acos(constrain(cosBeta, -1.0f, 1.0f)) * RAD_TO_DEG;
    float theta1 = 90.0f - (betaDeg + alphaDeg); //changed degrees to bring referenced from z axis to match other struts
    
    //Solve for Wrist
    float theta3 = 90 - theta1 - theta2 - phi;
    
    //RESULTS
    float servoTurret = 90.0f + turretDegrees;
    float elbowAngle = theta2; // REVIEW
    float shoulderAngle = 90.0f + theta1;
    float wristAngle = 90.0f + theta3;

    result.turret = constrain(servoTurret, MIN_ANGLE[0], MAX_ANGLE[0]);
    result.elbow = constrain(elbowAngle, MIN_ANGLE[2], MAX_ANGLE[2]);
    result.shoulder = constrain(shoulderAngle, MIN_ANGLE[1], MAX_ANGLE[1]);
    result.wrist = constrain(wristAngle, MIN_ANGLE[3], MAX_ANGLE[3]);

    Serial.println("---- IK DEBUG ----");
    Serial.print("rw="); Serial.println(rw);
    Serial.print("zw="); Serial.println(zw);
    Serial.print("d=");  Serial.println(d);

    Serial.print("alphaDeg=");  Serial.println(alphaDeg);
    Serial.print("betaDeg=");   Serial.println(betaDeg);
    Serial.print("theta1=");    Serial.println(theta1);
    Serial.print("theta2=");    Serial.println(theta2);
    Serial.print("theta3=");    Serial.println(theta3);

    Serial.print("servoTurret=");   Serial.println(servoTurret);
    Serial.print("shoulderAngle="); Serial.println(shoulderAngle);
    Serial.print("elbowAngle=");    Serial.println(elbowAngle);
    Serial.print("wristAngle=");    Serial.println(wristAngle);
    Serial.println("------------------");

    Serial.print("FINAL shoulder="); Serial.println(result.shoulder);
    Serial.print("FINAL elbow=");    Serial.println(result.elbow);
    Serial.print("FINAL wrist=");    Serial.println(result.wrist);

    /*
      
    */

    return result;
  }
ArmAngles IK::solveFullArmDualPassIK(float x, float y, float z, float phi)
{
    ArmAngles result;

    // -----------------------------
    // 1. TURRET
    // -----------------------------
    float turretDegrees = atan2(y, x) * RAD_TO_DEG;

    // -----------------------------
    // 2. TARGET → CLAW TIP
    // -----------------------------
    float rTarget = sqrt(x*x + y*y);

    // -----------------------------
    // PASS 1 — assume theta3 = phi
    // -----------------------------
    float theta3_guess = phi;   // temporary

    float rw = rTarget - (L3 * cos(theta3_guess * DEG_TO_RAD));
    float zw = z       - (L3 * sin(theta3_guess * DEG_TO_RAD));
    float d  = sqrt(rw*rw + zw*zw);

    // -----------------------------
    // Solve theta2 (elbow)
    // -----------------------------
    float cosGamma = (L1*L1 + L2*L2 - d*d) / (2.0f * L1 * L2);
    float innerGamma = acos(constrain(cosGamma, -1.0f, 1.0f)) * RAD_TO_DEG;
    float theta2 = 180.0f - innerGamma;

    // -----------------------------
    // Solve theta1 (shoulder)
    // -----------------------------
    float alphaDeg = atan2(zw, rw) * RAD_TO_DEG;
    float cosBeta  = (L1*L1 + d*d - L2*L2) / (2.0f * L1 * d);
    float betaDeg  = acos(constrain(cosBeta, -1.0f, 1.0f)) * RAD_TO_DEG;

    float theta1 = 90.0f - (betaDeg + alphaDeg);

    // -----------------------------
    // PASS 2 — compute REAL theta3
    // phi = (beta + alpha) - theta2 - theta3
    // beta + alpha = 90 - theta1
    // -----------------------------
    float theta3 = (90.0f - theta1) - theta2 - phi;

    // -----------------------------
    // PASS 3 — recompute wrist pivot using REAL theta3
    // -----------------------------
    float clawDir = (theta1 + theta2 + theta3) * DEG_TO_RAD;

    rw = rTarget - (L3 * cos(clawDir));
    zw = z       - (L3 * sin(clawDir));
    d  = sqrt(rw*rw + zw*zw);

    // -----------------------------
    // PASS 4 — recompute theta1, theta2 with corrected rw/zw
    // -----------------------------
    cosGamma = (L1*L1 + L2*L2 - d*d) / (2.0f * L1 * L2);
    innerGamma = acos(constrain(cosGamma, -1.0f, 1.0f)) * RAD_TO_DEG;
    theta2 = 180.0f - innerGamma;

    alphaDeg = atan2(zw, rw) * RAD_TO_DEG;
    cosBeta  = (L1*L1 + d*d - L2*L2) / (2.0f * L1 * d);
    betaDeg  = acos(constrain(cosBeta, -1.0f, 1.0f)) * RAD_TO_DEG;

    theta1 = 90.0f - (betaDeg + alphaDeg);

    // -----------------------------
    // 5. Convert joint angles → servo angles
    // -----------------------------
    float servoTurret   = 90.0f + turretDegrees;
    float shoulderAngle = 90.0f + theta1;
    float elbowAngle    = theta2;
    float wristAngle    = 90.0f + theta3;   // IMPORTANT

    result.turret   = constrain(servoTurret,   MIN_ANGLE[0], MAX_ANGLE[0]);
    result.shoulder = constrain(shoulderAngle, MIN_ANGLE[1], MAX_ANGLE[1]);
    result.elbow    = constrain(elbowAngle,    MIN_ANGLE[2], MAX_ANGLE[2]);
    result.wrist    = constrain(wristAngle,    MIN_ANGLE[3], MAX_ANGLE[3]);

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
    float phi = values[3];

    // Confirm parsed values
    Serial.print("PARSED: ");
    Serial.print(x); Serial.print(" ");
    Serial.print(y); Serial.print(" ");
    Serial.print(z); Serial.print(" ");
    Serial.println(phi);

    // Run IK
    //ArmAngles angles = solveFullArm(x, y, z, phi);
    ArmAngles angles = solveFullArmDualPassIK(x, y, z, phi);

    controller.turnServo(0, angles.turret);
    controller.turnServo(1, angles.shoulder);
    controller.turnServo(2, angles.elbow);
    controller.turnServo(3, angles.wrist);

    Serial.println("Target Updated");
}
