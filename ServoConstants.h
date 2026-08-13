#pragma once

#ifndef ServoConstants_h
#define ServoConstants_h

// Turret Shoulder Elbow Wrist Claw
// 0      1        2     3     4
const int MIN_TICK[5]        = {90, 90, 87, 85, 300};
const int MAX_TICK[5]        = {540, 545, 545, 533, 540};
const float MIN_ANGLE[5]     = {0.0f, 0.0f, 0.0f, 0.0f, 80.0f};
const float MAX_ANGLE[5]     = {180.0f, 150.0f, 180.0f, 180.0f, 175.0f};
const float HOMING[5]        = {90.0f, 140.0f, 180.0f, 0.0f, 175.0f};
const float HOMING_XYZPHI[4] = {0.0f, 0.0f, 0.0f, 0.0f};
const int servoArray[5]      = {0, 1, 2, 3, 4};

//IMPORTANT SERVO VALUES
const float ALLOWABLE_DEADZONE[5] = {0.2f, 0.2f, 0.2f, 0.15f, 0.1f};
const float SNAP_THRESHOLD[5]     = {1.0f, 0.7f, 0.7f, 0.4f, 0.2f};
const float P[5]                  = {1.8f, 2.2f, 2.0f, 2.5f, 2.5f};
const float D[5]                  = {0.12f, 0.22f, 0.20f, 0.08f, 0.05f};
const float MAX_VELO[5]           = {180.0f, 140.0f, 140.0f, 110.0f, 90.0f};

//ARM LENGTHS
const float L1 = 70.0f; //Shoulder to Elbow
const float L2 = 85.0f; //Elbow to Wrist
const float L3 = 84.0f; //Wrist to Claw

//Colinear Constant Angles
const float shoulderColinear = 95.0f;
const float elbowColinear    = 60.0f;
const float wristColinear    = 90.0f;

constexpr float MIN_X = -200.0f;
constexpr float MAX_X =  200.0f;
constexpr float MIN_Y =  0.0f;
constexpr float MAX_Y =  200.0f;
constexpr float MIN_Z =  -55.0f;
constexpr float MAX_Z =  200.0f;

/*



*/

#endif
