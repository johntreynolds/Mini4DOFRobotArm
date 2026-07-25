#pragma once

#ifndef ServoConstants_h
#define ServoConstants_h

//SERVO LOCATIONS - FIXED
// Turret Shoulder Elbow Wrist Claw
// 0      1        2     3     4
const int MIN_TICK[5]    = {90, 90, 87, 85, 85};
const int MAX_TICK[5]    = {540, 545, 545, 533, 540};
const float MIN_ANGLE[5]   = {0.0f, 0.0f, 0.0f, 0.0f, 157.0f};
const float MAX_ANGLE[5]   = {180.0f, 150.0f, 180.0f, 180.0f, 180.0f};
const float HOMING[5]      = {90.0f, 140.0f, 180.0f, 0.0f, 180.0f};
const int servoArray[5]  = {0, 1, 2, 3, 4};

//IMPORTANT SERVO VALUES
const float ALLOWABLE_DEADZONE[5] = {2, 1, 1, .5, .5}; //Servo Deadzones for PD Controller
const float SNAP_THRESHOLD[5] = {1.0, 0.7, 0.7, 0.4, 0.2};
const float P[5] = {1.0, 1.2, 1.2, 0.8, 0.6};
const float D[5] = {0.05, 0.08, 0.08, 0.04, 0.03};
const float MAX_VELO[5] = {80, 60, 60, 45, 30};

//Variable Arrays
inline float targetAngle[5]      = {90.0, 180.0, 180.0, 160.0, 180.0};
inline float virtualAngle[5]     = {90.0, 180.0, 180.0, 160.0, 180.0};
inline float lastError[5]        = {0.0, 0.0, 0.0, 0.0, 0.0};
inline unsigned long lastTime[5] = {0, 0, 0, 0, 0};

//ARM LENGTHS
const float L1 = 70.0; //Shoulder to Elbow
const float L2 = 85.0; //Elbow to Wrist
const float L3 = 84.0; //Wrist to Claw

//Colinear Constant Angles
const float shoulderColinear = 90;
const float elbowColinear = 90;
const float wristColinear = 90;

/*



*/

#endif
