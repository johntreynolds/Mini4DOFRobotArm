#include <Wire.h>
#include "ServoController.h"
#include "ServoConstants.h"

//--------------------------------------------------READY THE SERVOS-------------------------------------------------

void ServoController::begin(int freq) 
  {
    pwm.begin();
    pwm.setPWMFreq(freq);
  }

void ServoController::prepServos(float angle)
{
    for (int i = 0; i < 5; i++)
      {
        // Set physical servo position
        turnServo(servoArray[i], angle);

        // Initialize PD controller state
        virtualAngle[i] = angle;
        targetAngle[i]  = angle;
        lastError[i]    = 0;
      }

    Serial.println("Servos initialized.");
    delay(2000);
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
    //Set Servo Limits
    targetAngle[channel] = constrain(angle, MIN_ANGLE[channel], MAX_ANGLE[channel]);
  }

void ServoController::moveServoPD(int channel, float kp, float kd) //kp = proportional gain; kd = derivative gain
  {
    //Find where we are and how far we need to go
    float error = targetAngle[channel] - virtualAngle[channel];

    //Find Time
    unsigned long now = micros();
    float dt = (now - lastTime[channel]) / 1e6;

    if (dt <= 0.0001) 
      {
        return; 
      }

    lastTime[channel] = now;

    //Creates math for our P and D values for the PD Controller
    float P = kp * error;
    float D = kd * (error - lastError[channel]);
    float output = P + D;

    //Limit speed of the PID
     output = constrain(output, -0.5, 0.5);

    //Sets our current angle to the old angle + the angle output we calculate with P and D
    virtualAngle[channel] += output;

    //Make sure we don't send it too far
    virtualAngle[channel] = constrain(virtualAngle[channel], 0, 180);

    //Actually moves the specific servo, only adds output. Output gets smaller over time to slow down servo movement over time
    turnServo(channel, virtualAngle[channel]);

    //Sets the error we calculate to the lastError array to find rate of change over time
    lastError[channel] = error;
  }

void ServoController::moveAllServosPD(float kp, float kd)
  {
    for (int i = 0; i < 5; i++)
      {
        moveServoPD(i, kp, kd);
      }
  }