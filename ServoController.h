#ifndef ServoController_h
#define ServoController_h

#include <Adafruit_PWMServoDriver.h>

class ServoController 
    {
        public:
            //READY SERVOS
            void begin(int freq);
            void prepServos(float angle);

            //PD Controller
            void turnServo(int channel, float angle);
            void setTargetPD(int channel, float angle);
            void moveServoPD(int channel, float kp, float kd);
            void moveAllServosPD(float kp, float kd);

            //Run Text Changes Project
            void liveCommand(int servoNum, int angle);
            void readSerialCommands();
            void runTextChanges();

        private:
            //OBJECT CREATION
            Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
            
            //SERVO LIMITS
            int MIN_TICK[5] = {90, 90, 87, 85, 85};
            int MAX_TICK[5] = {540, 545, 545, 533, 540};
            const int MIN_ANGLE = 0;
            const int MAX_ANGLE = 180;

            //CHANNEL MAPPING
            const int DIFFY_LEFT = 0;
            const int DIFFY_RIGHT = 1;
            const int ELBOW = 2;
            const int WRIST = 3;
            const int CLAW = 4;
            const int servoArray[5] = {0, 1, 2, 3, 4};

            //PD Controller
            float targetAngle[5];
            float virtualAngle[5]; 
            float lastError[5];
            float P = .3;
            float D = .1;
    };

#endif