#include "MovementControl.h"

//------------------------------------------PREP ITEMS-----------------------------------------------

void MovementControl::begin(int freq) 
  {
    Serial.begin(115200);
    Wire.begin(21, 22);
    pwm.begin();
    delay(20);
    pwm.setPWMFreq(freq);
  }

void MovementControl::homeArm()
  {
    pd.resetPDToHome();
    for (int i = 0; i < 5; i++)
      {
        turnServo(i, HOMING[i]);
      }
  }

void MovementControl::telemetry()
  {
    Serial.println("---- TELEMETRY ----");
    Serial.print("RW:   "); Serial.println(angles.rw);
    Serial.print("ZW:   "); Serial.println(angles.zw);
    Serial.print("Turret:   "); Serial.println(angles.turret);
    Serial.print("Shoulder: "); Serial.println(angles.shoulder);
    Serial.print("Elbow:    "); Serial.println(angles.elbow);
    Serial.print("Wrist:    "); Serial.println(angles.wrist);
    if (angles.hasPhi == true)
      {
        Serial.print("Phi:    "); Serial.println(angles.phi);
      }
    Serial.print("Input XYZ: ");
    Serial.print(input.x); Serial.print(" ");
    Serial.print(input.y); Serial.print(" ");
    Serial.println(input.z);
    Serial.println("-------------------");
  }

//---------------------------------MOVE SERVOS------------------------------

void MovementControl::turnServo(int channel, float angle)
  {
    angle = constrain(angle, MIN_ANGLE[channel], MAX_ANGLE[channel]);
    int tick = map(angle, MIN_ANGLE[channel], MAX_ANGLE[channel], MIN_TICK[channel], MAX_TICK[channel]);
    pwm.setPWM(channel, 0, tick);
  }

void MovementControl::moveAllServos()
  {
    for (int i = 0; i < 5; i++)
      {
        pd.pdServoMath(i);
        float angle = pd.getVirtualAngle(i);
        turnServo(i, angle);
      }
  }

//----------------------------------------------------IK TESTING DEVELOPMENT-------------------------------------------

bool MovementControl::processIKSerial()
  {
    if (!Serial.available())
        return false;

    // Read full line
    String line = Serial.readStringUntil('\n');
    line.trim();

    // Normalize separators
    line.replace(',', ' ');
    line.replace('\t', ' ');
    line.replace('\r', ' ');
    line.trim();

    // Parse values
    float values[4];
    int count = 0;

    int start = 0;
    while (count < 4)
    {
        int spaceIndex = line.indexOf(' ', start);

        String token;
        if (spaceIndex == -1)
            token = line.substring(start);
        else
            token = line.substring(start, spaceIndex);

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

    if (count != 3 && count != 4)
    {
        Serial.print("Parse error: got ");
        Serial.print(count);
        Serial.println(" values");
        return false;
    }

    input.x = values[0];
    input.y = values[1];
    input.z = values[2];

    input.hasPhi = (count == 4);
    if (input.hasPhi)
      {
        input.phi = values[3];
      }
        
    return true;

  }

void MovementControl::manualIKTest()
  {
    if (!processIKSerial())
        return;
    if (input.hasPhi)
      {
        angles = ik.solveFullArmPhi(input.x, input.y, input.z, input.phi);
      }
    else
      {
        angles = ik.solveFullArmVert(input.x, input.y, input.z);
      }
    IKStatus status = ik.validateAngles(angles);
    if (status != IK_OK)
      {
          Serial.print("IK ERROR: ");
          Serial.println(status);
          return;   // SAFETY: DO NOT MOVE
      }

    telemetry();
    pd.setTargetPD(0, angles.turret);
    pd.setTargetPD(1, angles.shoulder);
    pd.setTargetPD(2, angles.elbow);
    pd.setTargetPD(3, angles.wrist);
    moveAllServos();
  }

void MovementControl::ikTestBox()
  {
    ArmAngles t;

        // ---- Pose 1 ----
        t = ik.solveFullArmHori(100, 0, 130);
        pd.setTargetPD(0, t.turret);
        pd.setTargetPD(1, t.shoulder);
        pd.setTargetPD(2, t.elbow);
        pd.setTargetPD(3, t.wrist);

        for (int i = 0; i < 50; i++)
          {
            moveAllServos();
            delay(5);
          }
        angles = t;
        telemetry();

        // ---- Pose 2 ----
        t = ik.solveFullArmHori(100, 0, 100);
        pd.setTargetPD(0, t.turret);
        pd.setTargetPD(1, t.shoulder);
        pd.setTargetPD(2, t.elbow);
        pd.setTargetPD(3, t.wrist);

        for (int i = 0; i < 50; i++)
          {
            moveAllServos();
            delay(5);
          }
        angles = t;
        telemetry();

        // ---- Pose 3 ----
        t = ik.solveFullArmHori(130, 0, 100);
        pd.setTargetPD(0, t.turret);
        pd.setTargetPD(1, t.shoulder);
        pd.setTargetPD(2, t.elbow);
        pd.setTargetPD(3, t.wrist);

        for (int i = 0; i < 50; i++)
          {
            moveAllServos();
            delay(5);
          }
        angles = t;
        telemetry();

        // ---- Pose 4 ----
        t = ik.solveFullArmHori(130, 0, 130);
        pd.setTargetPD(0, t.turret);
        pd.setTargetPD(1, t.shoulder);
        pd.setTargetPD(2, t.elbow);
        pd.setTargetPD(3, t.wrist);

        for (int i = 0; i < 50; i++)
          {
            moveAllServos();
            delay(5);
          }
        angles = t;
        telemetry();
  }

void MovementControl::rotateAroundXYZ(float x, float y, float z, float phiMin, float phiMax)
  {
    
  }

bool MovementControl::processWebTarget(float x, float y, float z, float phi) 
{
    // Pass 4 arguments and receive ArmAngles directly:
    ArmAngles targetAngles = ik.solveFullArmPhi(x, y, z, phi);

    // Update member variables
    angles = targetAngles;

    input.x = x;
    input.y = y;
    input.z = z;
    input.phi = phi;
    input.hasPhi = true;

    // Update PID target angles
    pd.setTargetPD(0, angles.turret);
    pd.setTargetPD(1, angles.shoulder);
    pd.setTargetPD(2, angles.elbow);
    pd.setTargetPD(3, angles.wrist);

    return true;
}