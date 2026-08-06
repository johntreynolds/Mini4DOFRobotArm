#pragma once

#ifndef CoreTypes_h
#define CoreTypes_h

enum IKStatus 
  {
      IK_OK,
      IK_UNREACHABLE,
      IK_TURRET_LIMIT,
      IK_SHOULDER_LIMIT,
      IK_ELBOW_LIMIT,
      IK_WRIST_LIMIT,
      IK_TOO_FAR,
      IK_TOO_CLOSE
  };

struct ArmAngles
  {
    float turret;
    float shoulder;
    float elbow;
    float wrist;
    float claw;
    IKStatus status;
  };

struct IKInput 
  {
    float x;
    float y;
    float z;
    bool valid; //MAYBE
  };

#endif

