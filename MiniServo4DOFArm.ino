#include "ServoController.h"
#include "ServoTesting.h"

//SETUP ITEMS
bool robotStopped = false;
ServoController armController;
ServoTesting testing;

//=====================================START OF PRODUCTION CODE FOR THE ROBOT======================================================

void setup() 
  {
    
    Serial.begin(9600);
    armController.begin(50);
    armController.prepServos(90);
    
    //SET SERVO POSITIONS FROM 0 to 180
    //testing.manualTickStep();
  }

void loop() 
  {
    armController.runTextChanges();
  }


  
