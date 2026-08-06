#include <Wire.h>
#include <math.h>

#include "PIDMath.h"

//----------------------------------PD Control Methods------------------------------------------------

void ServoController::setTargetPD(int channel, float angle)
  {
    angle = constrain(angle, MIN_ANGLE[channel], MAX_ANGLE[channel]);
    targetAngle[channel] = angle;
  }

void ServoController::moveServoPD(int channel)
  {
    //Find where we are and how far we need to go
    float error = targetAngle[channel] - virtualAngle[channel];

    if (fabs(error) < ALLOWABLE_DEADZONE[channel])
      {
        lastError[channel] = 0;
        return;
      }

    //Find Time
    unsigned long now = micros();
    float dt = (now - lastTime[channel]) / 1e6;
    lastTime[channel] = now;

    if (dt < .001f)
      {
        return; 
      }

    //Creates math for our P and D values for the PD Controller
    float pTerm = P[channel] * error;
    float dTerm = D[channel] * (error - lastError[channel]) / dt;
    float output = pTerm + dTerm;

    //Limit speed of the PID
    float maxStep = MAX_VELO[channel] * dt;
    output = constrain(output, -maxStep, maxStep);

    //Sets our current angle to the old angle + the angle output we calculate with P and D
    virtualAngle[channel] += output;

    //Make sure we don't send it too far
    virtualAngle[channel] = constrain(virtualAngle[channel], MIN_ANGLE[channel], MAX_ANGLE[channel]);

    error = targetAngle[channel] - virtualAngle[channel];
    
    //SNAP ERRORS
    if (fabs(error) < SNAP_THRESHOLD[channel])
      {
        virtualAngle[channel] = targetAngle[channel];
        turnServo(channel, virtualAngle[channel]);
        lastError[channel] = 0;
        return;
      }

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