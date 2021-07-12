#include "MyStepper.h"
#include "MyParseNumber.h"

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
        return ((PIND & (1 << pin)) >> pin);
    case 'B':
        pin -= 8;
        return ((PINB & (1 << pin)) >> pin);
    }

}

void sweep(Stepper *stepper, uint8_t pin1, uint8_t pin2, long *len){ 
    pinMode(pin1, INPUT_PULLUP);
    pinMode(pin2, INPUT_PULLUP);

    stepper->setDirection(LOW); 
    while(readPin(pin1) && readPin(pin2))
        stepper->step(3);
    stepper->setPosition(0); // set one boundary as 0 position.
    
    uint8_t otherPin;

    if (readPin(pin1))
        otherPin = pin1;
    else if (readPin(pin2))
        otherPin = pin2;

    stepper->setDirection(HIGH);
    while(readPin(otherPin))
        stepper->step(3);
    
    *len = stepper->getPosition();
    stepper->setPosition(length/2); // set centre as 0 position.
    stepper->moveTo(0);
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(500000);

    // centre the table. 
    // button at bound must be LOW when pressed, HIGH otherwise.
    sweep(&stepper1, left_pin, right_pin, &length);
}

void loop() {
    // put your main code here, to run repeatedly:

    // actual code
    if (count%10==0 && Serial.available() > 0){
        incomingBytes = myParseInt();
        stepper1.moveTo(incomingBytes);
        count = 0;
    }
    if (readPin(left_pin) && readPin(right_pin))
        stepper1.run(3); // caution, delay under 3 is inaccurate.
    
    count += 1;
}

