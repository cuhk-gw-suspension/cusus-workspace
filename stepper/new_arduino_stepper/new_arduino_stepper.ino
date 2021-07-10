#include "MyStepper.h"
#include "MyParseNumber.h"

/* unsigned long parseStart; */
/* unsigned long parsetime; */
/* unsigned long printStart; */
/* unsigned long printTime; */

long incomingBytes;
long length;
unsigned int count = 0;

// pins that denote the table have reached boundary
uint8_t left_pin = 12;
uint8_t right_pin = 11;

Stepper stepper1(2, 3); //init pul pin=2, dir pin=3

bool readPin(uint8_t pin){
    char sector;
    if (pin < 8)
        sector = 'D';
    else if (pin < 14)
        sector = 'B';

    switch (sector)
    {
    case 'D':
        return PIND && (1 << pin);
    case 'B':
        pin -= 8;
        return PINB && (1 << pin);
    }

}

void sweep(Stepper stepper, uint8_t pin1, uint8_t pin2, long *len){ 
    char sector;
    if (pin < 8)
        sector = 'D';
    else if (pin < 14)
        sector = 'B';

    switch (sector)
    {
    case 'D':
        DDRD &= ~(1 << pin);
        break;
    case 'B':
        pin -= 8;
        DDRB &= ~(1 << pin);
        break
    }

    stepper.setDirection(LOW); 
    while(readPin(pin1) && readPin(pin2))
        stepper.step(3);
    stepper.setPosition(0); // set one boundary as 0 position.

    stepper.setDirection(HIGH);
    while(readPin(pin1) && readPin(pin2))
        stepper.step(3);
    
    *len = stepper.getPosition();
    stepper.setPosition(length/2); // set centre as 0 position.
    stepper.moveTo(0);
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(500000);

    // centre the table. 
    // button at bound must be LOW when pressed, HIGH otherwise.
    sweep(stepper1, left_pin, right_pin, &length);
}

void loop() {
    // put your main code here, to run repeatedly:

    // actual code
    if (count%10==0 && Serial.available() > 0){
        incomingBytes = myParseInt();
        stepper1.moveTo(incomingBytes);
        count = 0;
    }
    if (readPin(pin1) && readPin(pin2))
        stepper1.run(3); // caution, delay under 3 is inaccurate.
    
    count += 1;


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

}

