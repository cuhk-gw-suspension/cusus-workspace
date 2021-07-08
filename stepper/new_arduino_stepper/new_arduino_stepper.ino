/* My self defined arduino stepper script

*/
#include "MyStepper.h"

unsigned long step_per_rev = 25600;

Stepper stepper1(2, 3); //init pul pin=2, dir pin=3

void setup() {
  // put your setup code here, to run once:
   stepper1.moveTo(step_per_rev*3); 
}

void loop() {
    // put your main code here, to run repeatedly:
    if (stepper1.distanceToGo() == 0)
        stepper1.moveTo(stepper1.getPosition());    
    stepper1.run();
}

