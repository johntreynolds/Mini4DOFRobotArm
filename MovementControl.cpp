#include "MovementControl.h"

extern RobotWebServer webServer;

//------------------------------------------PREP ITEMS-----------------------------------------------

void MovementControl::begin(int freq) 
  {
    Serial.begin(115200);
    Wire.begin(21, 22);
    pwm.begin();
    delay(20);
    pwm.setPWMFreq(freq);
    _isEStopped = false;
  }

void MovementControl::homeArm()
  {
    pd.resetPDToHome();
    IKInput homeState = ik.solveFullArmDK(HOMING[0], HOMING[1], HOMING[2], HOMING[3]);

    input.x = homeState.x;
    input.y = homeState.y;
    input.z = homeState.z;
    input.phi = homeState.phi;
    input.hasPhi = homeState.hasPhi;
    angles.turret   = HOMING[0];
    angles.shoulder = HOMING[1];
    angles.elbow    = HOMING[2];
    angles.wrist    = HOMING[3];
    angles.claw     = HOMING[4];

    for (int i = 0; i < 5; i++)
      {
        turnServo(i, HOMING[i]);
      } 
    
    webServer.sendTelemetry(input.x, input.y, input.z, input.phi, angles.turret, angles.shoulder, angles.elbow, angles.wrist, angles.claw);
    webServer.log("Robot Homed");
  }

//---------------------------------MOVE SERVOS------------------------------

void MovementControl::turnServo(int channel, float angle)
  {
    if (_isEStopped)
      {
        return;
      }
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

/*
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

*/

// --------------------------------- MOVEMENT HANDLING ----------------------------

/*
*/

void MovementControl::emergencyStop()
  {
    _isEStopped = true;
    for (int i = 0; i < 5; i++)
      {
        pwm.setPWM(i, 0, 0);
      }
    // TEXT FOR EMERGENCY STOP 
  }

void MovementControl::releaseEStop()
  {
    _isEStopped = false;
  }

void MovementControl::setOpMode(uint8_t mode)
  {
    if (_currentOpMode == mode)
      {
        return;
      }
    _currentOpMode = mode;

    if (_currentOpMode == RobotWebServer::OPMODE_REMOTE_CONTROL)
      {
        syncRCTargetsFromCurrentPose();
      }
  }

void MovementControl::getCurrentPose(float &x, float &y, float &z, float &phi)
  {
    x = input.x;
    y = input.y;
    z = input.z;
    phi = input.hasPhi ? input.phi : 0.0f;
  }

void MovementControl::getCurrentAngles(float &turret, float &shoulder, float &elbow, float &wrist, float &claw)
  {
    turret = angles.turret;
    shoulder = angles.shoulder;
    elbow = angles.elbow;
    wrist = angles.wrist;
    claw = angles.claw;
  }

// ----------------------------- CLAW MOVEMENT ------------------------------

void MovementControl::moveClaw(float percent)
  {
    percent = constrain(percent, 0.0f, 100.0f);
    float clawAngle = map(percent, 0.0f, 100.0f, MAX_ANGLE[4], MIN_ANGLE[4]);
    angles.claw = clawAngle;
    turnServo(4, clawAngle);
  }

// ----------------------------------- OPMODES ----------------------------------------

bool MovementControl::rotateAroundXYZ(float x, float y, float z, float phiMin, float phiMax)
  {
    
  }

bool MovementControl::manualCompute(float x, float y, float z, float phi)
  {
    ArmAngles targetAngles = ik.solveFullArmPhi(x, y, z, phi);
    IKStatus status = ik.validateAngles(targetAngles);
    if (status != IK_OK)
      {
        webServer.logf("[IK ERROR] Manual XYZ target (%.1f, %.1f, %.1f) unreachable! Code: %d\n", 
                      x, y, z, status);
        return false;
      }
    // Update state & PD targets
    angles = targetAngles;
    input.x = x; 
    input.y = y; 
    input.z = z; 
    input.phi = phi; 
    input.hasPhi = true;

    pd.setTargetPD(0, angles.turret);
    pd.setTargetPD(1, angles.shoulder);
    pd.setTargetPD(2, angles.elbow);
    pd.setTargetPD(3, angles.wrist);

    moveAllServos();
    return true;

  }

bool MovementControl::runManualXYZ(float x, float y, float z, float phi, float clawPercent)
  {
    if (!manualCompute(x, y, z, phi)) 
      {
        return false; 
      }

    moveClaw(clawPercent);
    return true; 
  }  

bool MovementControl::runLiveSliders(float x, float y, float z, float phi, float clawPercent)
  {
    if (!manualCompute(x, y, z, phi)) 
      {
        return false;
      }
    moveClaw(clawPercent);
    return true;
  }

bool MovementControl::manualAngleMove(float turret, float shoulder, float elbow, float wrist, float claw)
  {
    if (_isEStopped)
      {
        return false;
      }

    IKInput dk = ik.solveFullArmDK(turret, shoulder, elbow, wrist);

    angles.turret   = turret;
    angles.shoulder = shoulder;
    angles.elbow    = elbow;
    angles.wrist    = wrist;
    angles.claw     = claw;

    input.x = dk.x;
    input.y = dk.y;
    input.z = dk.z;
    input.phi = dk.phi;
    input.hasPhi = dk.hasPhi;

    webServer.sendTelemetry(dk.x, dk.y, dk.z, dk.phi, turret, shoulder, elbow, wrist, claw);

    turnServo(0, turret);
    turnServo(1, shoulder);
    turnServo(2, elbow);
    turnServo(3, wrist);
    turnServo(4, claw);

    pd.setTargetPD(0, turret);
    pd.setTargetPD(1, shoulder);
    pd.setTargetPD(2, elbow);
    pd.setTargetPD(3, wrist);

    return true;
  }

// -------------------------------------------- RC MODE ---------------------------------------

float MovementControl::applyDeadzone(float input) 
  {
    const float dz = 0.12f;
    float absVal = abs(input);
    
    if (absVal < dz) {
      return 0.0f;
    }
    
    // Rescale linearly above deadzone threshold
    float scaled = (absVal - dz) / (1.0f - dz);
    
    // Apply exponential curve for fine control near center
    float expScaled = pow(scaled, 1.8f);
    
    return (input < 0.0f) ? -expScaled : expScaled;
  }

void MovementControl::syncRCTargetsFromCurrentPose() 
  {
    // Derive initial R and ThetaT targets from current X, Y, Z, Phi
    targetThetaT = angles.turret;
    targetZ = input.z;
    targetPhi = input.phi;
    targetR = sqrt((input.x * input.x) + (input.y * input.y));
    targetClaw = angles.claw;
  }

void MovementControl::handleRCCommand(const RCInputs& rc) 
  {
    const float dt = 0.020f;

    // 1. Immediate E-Stop Check ('B' Button)
    if (rc.btnEStop) 
      {
        emergencyStop();
        return;
      }

    if (_isEStopped || _currentOpMode != RobotWebServer::OPMODE_REMOTE_CONTROL) 
      { 
        return;
      }

    // 2. Home Arm Check ('X' Button)
    if (rc.btnHome) 
      {
        homeArm();
        syncRCTargetsFromCurrentPose(); // Keep RC targets aligned with home pose
        return;
      }

    // =========================================================================
    // STEP A: Store current targets before applying joystick increments
    // =========================================================================
    float prevTargetR      = targetR;
    float prevTargetZ      = targetZ;
    float prevTargetThetaT = targetThetaT;
    float prevTargetPhi    = targetPhi;

    // 3. Process Analog Sticks (Left Stick: R/Z, Right Stick: Turret)
    float moveR      = applyDeadzone(rc.lx);
    float moveZ      = -applyDeadzone(rc.ly);
    float moveTurret = applyDeadzone(rc.rx);

    float speedR      = 200.0f; // mm / sec
    float speedZ      = 200.0f; // mm / sec
    float speedTurret = 400.0f; // deg / sec

    targetR      += moveR * speedR * dt;
    targetZ      += moveZ * speedZ * dt;
    targetThetaT += moveTurret * speedTurret * dt;

    const float MIN_ARM_REACH_MM = 30.0f;
    const float MAX_ARM_REACH_MM = L1 + L2 + L3; // Set this below the physical limit where IK breaks

    targetR      = constrain(targetR, MIN_ARM_REACH_MM, MAX_ARM_REACH_MM);
    targetZ      = constrain(targetZ, -100.0f, 180.0f);
    targetThetaT = constrain(targetThetaT, 0.0f, 180.0f);

    // 4. Process D-Pad for Pitch Angle (Phi)
    float speedPhi = 100.0f; // deg / sec
    if (rc.dpadUp) 
      {
        targetPhi += speedPhi * dt;
      }
    if (rc.dpadDown) 
      {
        targetPhi -= speedPhi * dt;
      }
    targetPhi = constrain(targetPhi, 0.0f, 180.0f);

    // 5. Process Triggers/Bumpers for Claw Differential
    float speedClaw = 45.0f; // deg / sec
    if (rc.openClaw > 0.05f) 
      {
        targetClaw -= rc.openClaw * speedClaw * dt;
      }
    if (rc.closeClaw > 0.05f) 
      {
        targetClaw += rc.closeClaw * speedClaw * dt;
      }
    targetClaw = constrain(targetClaw, MIN_ANGLE[4], MAX_ANGLE[4]);

    // 6. Compute Cylindrical IK directly via IKMath instance
    ArmAngles targetAngles = ik.manualComputeRZ(targetThetaT, targetR, targetZ, targetPhi);

    // 7. Validate & Execute
    IKStatus status = ik.validateAngles(targetAngles);
    if (status == IK_OK)
      {
        angles = targetAngles;
        input.x = targetR; 
        input.y = 0.0f; 
        input.z = targetZ; 
        input.phi = targetPhi; 
        input.hasPhi = true;

        /*
        
        turnServo(0, angles.turret);
        turnServo(1, angles.shoulder);
        turnServo(2, angles.elbow);
        turnServo(3, angles.wrist);
        
        */

        pd.setTargetPD(0, angles.turret);
        pd.setTargetPD(1, angles.shoulder);
        pd.setTargetPD(2, angles.elbow);
        pd.setTargetPD(3, angles.wrist);
        moveAllServos();
       
        turnServo(4, targetClaw); // Update claw
      }
    else
      {
        // =========================================================================
        // STEP B: Restore previous targets so numbers don't change on failure
        // =========================================================================
        targetR      = prevTargetR;
        targetZ      = prevTargetZ;
        targetThetaT = prevTargetThetaT;
        targetPhi    = prevTargetPhi;

        webServer.logf("[IK ERROR] Target (T=%.1f, R=%.1f, Z=%.1f) unreachable! Code: %d\n", 
                       targetThetaT, targetR, targetZ, status);
      }
  }