#pragma once

<<<<<<< Updated upstream
const int MIN_TICK[5]    = {90, 90, 87, 85, 85};
const int MAX_TICK[5]    = {540, 545, 545, 533, 540};
const int MIN_ANGLE[5]   = {0, 0, 0, 0, 0};
const int MAX_ANGLE[5]   = {180, 180, 180, 180, 180};
const int servoArray[5]  = {0, 1, 2, 3, 4};
=======
//SERVO LOCATIONS - FIXED
// Turret Shoulder Elbow Wrist Claw
// 0      1        2     3     4
const int MIN_TICK[5]    = {90, 90, 87, 85, 85};
const int MAX_TICK[5]    = {540, 545, 545, 533, 540};
const int MIN_ANGLE[5]   = {0, 0, 0, 0, 80};
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

//90 Degree Offset Constants
const float shoulderOffset = 0.0;
const float elbowOffset = 0.0;
const float wristOffset = 0.0;
>>>>>>> Stashed changes

/*

#pragma once

const int MIN_TICK[5]    = {90, 90, 87, 85, 85};
const int MAX_TICK[5]    = {540, 545, 545, 533, 540};
const int MIN_ANGLE[5]   = {0, 0, 0, 0, 0};
const int MAX_ANGLE[5]   = {180, 180, 180, 180, 180};
const int servoArray[5]  = {0, 1, 2, 3, 4};

const char* ACTUATOR_NAMES[5] = {
    "Turret", "Shoulder", "Elbow", "Wrist", "Claw"
};


*/
