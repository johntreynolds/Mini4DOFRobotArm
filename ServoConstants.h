#pragma once

#ifndef ServoConstants_h
#define ServoConstants_h

// Turret Shoulder Elbow Wrist Claw
// 0      1        2     3     4
const int MIN_TICK[5]    = {90, 90, 87, 85, 85};
const int MAX_TICK[5]    = {540, 545, 545, 533, 540};
const float MIN_ANGLE[5]   = {0.0f, 0.0f, 0.0f, 0.0f, 167.5f};
const float MAX_ANGLE[5]   = {180.0f, 150.0f, 180.0f, 180.0f, 180.0f};
const float HOMING[5]      = {90.0f, 140.0f, 180.0f, 0.0f, 180.0f};
const int servoArray[5]  = {0, 1, 2, 3, 4};

//IMPORTANT SERVO VALUES
const float ALLOWABLE_DEADZONE[5] = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f}; //Servo Deadzones for PD Controller
const float SNAP_THRESHOLD[5] = {1.0f, 0.7f, 0.7f, 0.4f, 0.2f};
const float P[5] = {1.0f, 1.2f, 1.2f, 0.8f, 0.6f};
const float D[5] = {0.05f, 0.08f, 0.08f, 0.04f, 0.03f};
const float MAX_VELO[5] = {80.0f, 60.0f, 60.0f, 45.0f, 30.0f};

//ARM LENGTHS
const float L1 = 70.0f; //Shoulder to Elbow
const float L2 = 85.0f; //Elbow to Wrist
const float L3 = 84.0f; //Wrist to Claw

//Colinear Constant Angles
const float shoulderColinear = 95.0f;
const float elbowColinear = 60.0f;
const float wristColinear = 90.0f;

/*



*/

#endif
