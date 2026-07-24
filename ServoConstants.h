#pragma once

#ifndef ServoConstants_h
#define ServoConstants_h

//SERVO LOCATIONS - FIXED
// Turret Shoulder Elbow Wrist Claw
// 0      1        2     3     4
const int MIN_TICK[5]    = {90, 90, 87, 85, 85};
const int MAX_TICK[5]    = {540, 545, 545, 533, 540};
const int MIN_ANGLE[5]   = {0, 0, 0, 0, 120};
const int MAX_ANGLE[5]   = {180, 180, 180, 180, 180};
const int HOMING[5]      = {90, 180, 180, 160, 180};
const int servoArray[5]  = {0, 1, 2, 3, 4};

//Variable Arrays
inline float targetAngle[5]      = {90.0, 180.0, 180.0, 160.0, 180.0};
inline float virtualAngle[5]     = {90.0, 180.0, 180.0, 160.0, 180.0};
inline float lastError[5]        = {0.0, 0.0, 0.0, 0.0, 0.0};
inline unsigned long lastTime[5] = {0, 0, 0, 0, 0};

//ARM LENGTHS
const float L1 = 70.0; //Shoulder to Elbow
const float L2 = 85.0; //Elbow to Wrist
const float L3 = 84.0; //Wrist to Claw

//90 Degree Offset Constants BASED ON |_ (offset right)
const float shoulderCenterOffset = 18.0;
const float elbowCenterOffset = 45.0;
const float wristCenterOffset = 0.0;

//Colinear Constant Angles
const float shoulderColinear = 115;
const float elbowColinear = 45;
const float wristColinear = 90;

/*



*/

#endif
