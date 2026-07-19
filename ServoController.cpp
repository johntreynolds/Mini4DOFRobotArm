#include <Wire.h>
#include "ServoController.h"

//--------------------------------------------------READY THE SERVOS-------------------------------------------------

void ServoController::begin(int freq) 
  {
    pwm.begin();
    pwm.setPWMFreq(freq);
  }

void ServoController::homeArm()
{
    //SET TIME
    unsigned long now = micros();
    for (int i = 1 ; i < 5; i++)
      {
        // Initialize PD controller state
        virtualAngle[i] = HOMING[i];
        targetAngle[i]  = HOMING[i];
        lastError[i]    = 0;
        lastTime[i]     = now;
      }
}

void ServoController::homeTurret()
{
    float target = HOMING[0];   // 90 degrees
    turnServo(0, target);
    // Sync PD so it doesn't snap afterward
    virtualAngle[0] = target;
    targetAngle[0]  = target;
    lastError[0]    = 0;
    lastTime[0]     = micros();
}

//----------------------------------PD Control Methods------------------------------------------------

void ServoController::turnServo(int channel, float angle)
  {
    angle = constrain(angle, MIN_ANGLE[channel], MAX_ANGLE[channel]);
    int tick = map(angle, MIN_ANGLE[channel], MAX_ANGLE[channel], MIN_TICK[channel], MAX_TICK[channel]);
    pwm.setPWM(channel, 0, tick);
  }

void ServoController::setTargetPD(int channel, float angle)
  {
    angle = constrain(angle, MIN_ANGLE[channel], MAX_ANGLE[channel]);
    targetAngle[channel] = angle;
  }

void ServoController::moveServoPD(int channel)
  {
    //Find where we are and how far we need to go
    float error = targetAngle[channel] - virtualAngle[channel];

    //Find Time
    unsigned long now = micros();
    float dt = (now - lastTime[channel]) / 1e6;
    lastTime[channel] = now;

    //Creates math for our P and D values for the PD Controller
    float pTerm = P * error;
    float dTerm = D * (error - lastError[channel]) / dt;
    float output = pTerm + dTerm;

    //Limit speed of the PID
     output = constrain(output, -0.5, 0.5);

    //Sets our current angle to the old angle + the angle output we calculate with P and D
    virtualAngle[channel] += output;

    //Make sure we don't send it too far
    virtualAngle[channel] = constrain(virtualAngle[channel], MIN_ANGLE[channel], MAX_ANGLE[channel]);

    //Actually moves the specific servo, only adds output. Output gets smaller over time to slow down servo movement over time
    turnServo(channel, virtualAngle[channel]);

    //Sets the error we calculate to the lastError array to find rate of change over time
    lastError[channel] = error;
  }

void ServoController::moveAllServosPD()
  {
    for (int i = 0; i < 5; i++)
      {
        moveServoPD(i);
      }
  }