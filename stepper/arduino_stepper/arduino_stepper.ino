#include <stdlib.h>
#include <Arduino.h>

// Define a stepper and the pins it will use
AccelStepper stepper1(AccelStepper::DRIVER, 2, 3); //Driver mode, pul: pin2, dir: pin3 
// Defaults of  AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

int steps_per_revolution = 25600;

void setup()
{  
  DDRD |= B11111100;
  
  // Change these to suit your stepper if you want
  stepper1.setMaxSpeed(steps_per_revolution*10.0);
  stepper1.setAcceleration(steps_per_revolution*5.0);
  stepper1.moveTo((long)steps_per_revolution*9);
  stepper1.setMinPulseWidth(1);
}

void loop()
{
    // If at the end of travel go to the other end
    if (stepper1.distanceToGo() == 0)
      stepper1.moveTo(-stepper1.currentPosition());

    stepper1.run();
}
