#ifndef ServoTesting_h
#define ServoTesting_h
#include <Adafruit_PWMServoDriver.h>

class ServoTesting
    {
        public:
            void begin(int freq);
            void stopServo(int channel);
            void manualTickStep();

            const int BBB_SERVO = 0;
            const int AD002_SERVO = 3;

        private:
            //OBJECT CREATION
            Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
            
            //CHANNEL MAPPING
            const int servoArray[5] = {0, 1, 2, 3, 4};
      
    };

#endif
