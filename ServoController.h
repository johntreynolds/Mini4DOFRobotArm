#ifndef ServoController_h
#define ServoController_h

#include <Adafruit_PWMServoDriver.h>

//OBJECT CREATION
extern Adafruit_PWMServoDriver pwm;
            
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

<<<<<<< Updated upstream
        private:
            //PD Controller
            float targetAngle[5];
            float virtualAngle[5]; 
            float lastError[5];
            float P = .3;
            float D = .1;
=======
        private: 
            //P and D GAIN
            float P = .8;
            float D = .3;
        
>>>>>>> Stashed changes
    };

#endif