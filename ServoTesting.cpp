#include <Wire.h>
#include "ServoTesting.h"
#include "ServoConstants.h"

// ------------------------------IMPORTANT ACTIONS----------------------------------

void ServoTesting::beginTest(int freq)
  {
      pwm.begin();
      pwm.setPWMFreq(freq);
  }

void ServoTesting::turnServosTest(int channel, float angle)
  {
    angle = constrain(angle, MIN_ANGLE[channel], MAX_ANGLE[channel]);
    int tick = map(angle, MIN_ANGLE[channel], MAX_ANGLE[channel], MIN_TICK[channel], MAX_TICK[channel]);
    pwm.setPWM(channel, 0, tick);
  }

void ServoTesting::prepServosTest(float angle)
    {
        for (int i = 0; i < 5; i++)
            {
            // Set physical servo position
            turnServosTest(servoArray[i], angle);
            }
        Serial.println("Servos initialized.");
        delay(2000);
    }


void ServoTesting::stopServo(int channel)
  {
    pwm.setPWM(channel, 0, 0);
  }

// ------------------------TUNING TICK RANGES------------------------------

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

//---------------------------RUN TEXT CHANGES PROJECT------------------------------------------

void ServoTesting::liveCommand(int servoNum, int angle)
  {
    // Special command: move ALL servos
    if (servoNum == 5)
      {
        for (int i = 0; i < 5; i++)
          { 
            turnServosTest(i, angle);
          }

        Serial.print("All servos set to ");
        Serial.println(angle);
        return;
      }

    // Normal single-servo command
    if (servoNum < 0 || servoNum >= 5)
      {
        Serial.println("Invalid servo number (must be 1–5, or 6 for ALL)");
        return;
      }

    turnServosTest(servoNum, angle);

    Serial.print("Servo ");
    Serial.print(servoNum + 1);
    Serial.print(" target set to ");
    Serial.println(angle);
  }

void ServoTesting::readSerialCommands()
  {
    static bool hasRunOnce = false;

    if (!hasRunOnce) {
        //Test Instructions
        Serial.println("Select a number 1-5 to move the servos.");
        Serial.println("Type in format <Servor> <Angle>");
        Serial.println("1. Turret");
        Serial.println("2. Shoulder");
        Serial.println("3. Elbow");
        Serial.println("4. Wrist");
        Serial.println("5. Claw");
        hasRunOnce = true;
    }
    if (Serial.available())
      {
        String line = Serial.readStringUntil('\n');

        int spaceIndex = line.indexOf(' ');
        if (spaceIndex == -1)
          {
            Serial.println("Format error. Use: servo angle");
            return;
          }

        int servoNum = line.substring(0, spaceIndex).toInt();
        int angle    = line.substring(spaceIndex + 1).toInt();

        liveCommand(servoNum - 1, angle);

      }
  }

void ServoTesting::runServoTravelTuning()
  {
    readSerialCommands();
  }

// ------------------------------I2C Test---------------------------

void ServoTesting::scanI2C()
    {
    Serial.println("=== I2C Scanner ===");

    byte error, address;
    int nDevices = 0;

    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            Serial.print("I2C device found at 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
            nDevices++;
        }
    }

    if (nDevices == 0)
        Serial.println("No I2C devices found.");
    else
        Serial.println("Scan complete.");

    Serial.println("====================");
    }


  

  