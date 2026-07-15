#include <Wire.h>
#include "ServoTesting.h"

void ServoTesting::begin(int freq)
  {
      pwm.begin();
      pwm.setPWMFreq(freq);
  }

void ServoTesting::stopServo(int channel)
  {
    pwm.setPWM(channel, 0, 0);
  }

void ServoTesting::manualTickStep()
{
    Serial.println("=== MANUAL TICK STEP MODE ===");
    Serial.println("Commands:");
    Serial.println("  + = tick up");
    Serial.println("  - = tick down");
    Serial.println("  n = next servo");
    Serial.println("  q = quit");
    Serial.println();

    int currentServo = 0;
    int tick = 300;   // start near center

    Serial.print("Controlling servo ");
    Serial.println(currentServo);

    pwm.setPWM(servoArray[currentServo], 0, tick);

    while (true)
    {
        if (Serial.available())
        {
            char cmd = Serial.read();

            if (cmd == '+')
            {
                tick++;
                if (tick > 4095) tick = 4095;
                pwm.setPWM(servoArray[currentServo], 0, tick);
                Serial.print("Tick: ");
                Serial.println(tick);
            }
            else if (cmd == '-')
            {
                tick--;
                if (tick < 0) tick = 0;
                pwm.setPWM(servoArray[currentServo], 0, tick);
                Serial.print("Tick: ");
                Serial.println(tick);
            }
            else if (cmd == 'n')
            {
                currentServo++;
                if (currentServo > 4)
                {
                    Serial.println("All servos done.");
                    break;
                }

                Serial.print("Switching to servo ");
                Serial.println(currentServo);

                tick = 300; // reset to center
                pwm.setPWM(servoArray[currentServo], 0, tick);
            }
            else if (cmd == 'q')
            {
                Serial.println("Exiting manual tick mode.");
                break;
            }
        }
    }
}

  

  