#include "MyStepper.h"
#include "MyParseNumber.h"

long incomingBytes;
long bound;
unsigned long time_now, time_old;

Stepper stepper1(2, 3); //init pul pin=2, dir pin=3


void setup() {
    // put your setup code here, to run once:
    Serial.begin(500000);
    
    /* stepper1.setSpeed( ); // number of steps per sec. */
    
    // centre the table. 
    // button at bound must be LOW when pressed, HIGH otherwise.
    stepper1.sweep(12, 10); // pins of limit switches
}

void loop() {
    // put your main code here, to run repeatedly:

    // actual code
    if (Serial.available() > 0){
        incomingBytes = myParseInt();
        stepper1.moveTo(incomingBytes);
    }
    
    stepper1.run(3); // caution, delay under 3 is inaccurate. 
}

