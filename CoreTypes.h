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

#endif

