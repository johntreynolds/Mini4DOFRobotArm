#pragma once

#ifndef CoreTypes_h
#define CoreTypes_h

enum IKStatus 
  {
      IK_TURRET_LIMIT,    //Error Code 0
      IK_SHOULDER_LIMIT,  //Error Code 1
      IK_ELBOW_LIMIT,     //Error Code 2
      IK_WRIST_LIMIT,     //Error Code 3
      IK_TOO_FAR,         //Error Code 4
      IK_TOO_CLOSE,       //Error Code 5
      IK_XYZ_LIMIT,
      IK_OK              
  };

struct ArmAngles
  {
    float turret;
    float shoulder;
    float elbow;
    float wrist;
    float claw;
    float rw;
    float zw;
    float phi;
    bool hasPhi;
    IKStatus status;
  };

struct IKInput 
  {
    float x;
    float y;
    float z;
    float phi;
    float hasPhi;
  };

struct RCInputs 
  {
    float lx;         // Left stick X (-1.0 R In, +1.0 R Out)
    float ly;         // Left stick Y (+1.0 Z Up, -1.0 Z Down)
    float rx;         // Right stick X (-1.0 Turret Left, +1.0 Turret Right)
        
    bool dpadUp;      // Phi Up
    bool dpadDown;    // Phi Down
        
    float openClaw;   // Left trigger/bumper (0.0 to 1.0)
    float closeClaw;  // Right trigger/bumper (0.0 to 1.0)
  };


#endif

