/* My self defined arduino stepper script

*/
#include "MyStepper.h"
#include "MyParseNumber.h"

long incomingBytes;
unsigned long parseStart;
unsigned long parsetime;
unsigned long printStart;
unsigned long printTime;

Stepper stepper1(2, 3); //init pul pin=2, dir pin=3

long tar = 25600;
unsigned int count = 0;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(500000); 
    stepper1.moveTo(tar*2);
}

void loop() {
    // put your main code here, to run repeatedly:

    // test stepper code
    /* if (stepper1.distanceToGo() == 0){ */
    /*     stepper1.moveTo(-stepper1.getPosition()); */ 
    /*     Serial.println("changed direction"); */
    /* } */
    /* stepper1.run(2); */

    // test serial r,w code
    /* if (Serial.available() > 0) */
    /* { */
    /*     parseStart = micros(); */
    /*     /1* incomingBytes = Serial.parseInt(); //takes about 15000 microsec *1/ */
    /*     incomingBytes = myParseInt(); */
    /*     parsetime = micros() - parseStart; */

    /*     printStart = micros(); */
    /*     Serial.println(incomingBytes); // takes about 500 microsec */
    /*     printTime = micros() - printStart; */

    /*     Serial.print(parsetime); */
    /*     Serial.print("\t"); */
    /*     Serial.println(printTime); */
    /* } */


    // actual code
    if (count%10==0 && Serial.available() > 0){
        incomingBytes = myParseInt();
        stepper1.moveTo(incomingBytes);
        count = 0;
    }
    stepper1.run(3); // caution, delay under 3 is inaccurate.
}

