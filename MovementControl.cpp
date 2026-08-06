#include "MovementControl.h"
#include "ServoConstants.h"

#include <Wire.h>
#include <math.h>

void MovementControl::begin(int freq) 
  {
    pwm.begin();
    pwm.setPWMFreq(freq);
  }

void MovementControl::turnServo(int channel, float angle)
  {
    angle = constrain(angle, MIN_ANGLE[channel], MAX_ANGLE[channel]);
    int tick = map(angle, MIN_ANGLE[channel], MAX_ANGLE[channel], MIN_TICK[channel], MAX_TICK[channel]);
    pwm.setPWM(channel, 0, tick);
  }

void MovementControl::homeArm()
{
    //SET TIME
    unsigned long now = micros();
    for (int i = 0 ; i < 5; i++)
      {
        // Initialize PD controller state
        virtualAngle[i] = HOMING[i];
        targetAngle[i]  = HOMING[i];
        lastError[i]    = 0;
        lastTime[i]     = now;
        turnServo(i, HOMING[i]);
      }
    Serial.println("Arm Homed");
}

void MovementControl::processIKSerial()
  {
    if (!Serial.available())
        return;

    // Read full line
    String line = Serial.readStringUntil('\n');
    line.trim();

    // Normalize separators
    line.replace(',', ' ');
    line.replace('\t', ' ');
    line.replace('\r', ' ');
    line.trim();

    // Parse values
    float values[3];
    int count = 0;

    int start = 0;
    while (count < 3)
    {
        int spaceIndex = line.indexOf(' ', start);

        String token;
        if (spaceIndex == -1)
            token = line.substring(start);
        else
            token = line.substring(start, spaceIndex);

        token.trim();

        if (token.length() > 0)
        {
            values[count] = token.toFloat();
            count++;
        }

        if (spaceIndex == -1)
            break;

        start = spaceIndex + 1;
    }

    if (count != 3)
    {
        Serial.print("Parse error: got ");
        Serial.print(count);
        Serial.println(" values");
        return;
    }

    input.x = values[0];
    input.y = values[1];
    input.z = values[2];

    // Telemetry (IK math stays clean)
    Serial.println("---- TELEMETRY ----");
    Serial.print("Input XYZ: ");
    Serial.print(input.x); Serial.print(" ");
    Serial.print(input.y); Serial.print(" ");
    Serial.println(input.z);

    Serial.print("Turret:   "); Serial.println(angles.turret);
    Serial.print("Shoulder: "); Serial.println(angles.shoulder);
    Serial.print("Elbow:    "); Serial.println(angles.elbow);
    Serial.print("Wrist:    "); Serial.println(angles.wrist);
    Serial.println("-------------------");
  }