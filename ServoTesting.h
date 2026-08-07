#ifndef ServoTesting_h
#define ServoTesting_h

#include <Adafruit_PWMServoDriver.h>

//OBJECT CREATION
extern Adafruit_PWMServoDriver pwm;

class ServoTesting
    {
        public:
            //Important Operations
            void moveToAngleTest(int channel, float angle);

            //Tuning for setting up pulse ranges
            void manualTickStep();

            //Run Text Changes Project
            void liveCommand(int servoNum, int angle);
            void readSerialCommands();
            void runServoTravelTuning();

            //I2C Test
            void scanI2C();

        private:
    };

#endif
