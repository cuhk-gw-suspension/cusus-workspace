#include "MyStepper.h"
#include "MyParseNumber.h"

long incomingBytes;
long length;
unsigned long time_now, time_old;

// pins that denote the table have reached boundary
const uint8_t left_pin = 12;
const uint8_t right_pin = 10;

const int dt = 10000; // interval between sample, in microsec.

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

void sweep(Stepper *stepper, uint8_t pin1, uint8_t pin2){ 
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
    long length = 0;
    stepper->setDirection(HIGH);
    while(readPin(otherPin)){
        stepper->step(3);
        length += 1;
    }
    
    stepper->setPosition(length/2); // set centre as 0 position.
    stepper->moveTo(0);
    while (stepper->distanceToGo() != 0){
        if (readPin(pin1) && readPin(pin2))
            stepper->run(3);
    }
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(500000);

    // centre the table. 
    // button at bound must be LOW when pressed, HIGH otherwise.
    sweep(&stepper1, left_pin, right_pin);
    time_old = micros();
}

void loop() {
    // put your main code here, to run repeatedly:

    // actual code
    time_now = micros();
    if (time_now - time_old >= dt && Serial.available() > 0){
        incomingBytes = myParseInt();
        stepper1.moveTo(incomingBytes);
        time_old = time_now;
    }
    if (readPin(left_pin) && readPin(right_pin))
        stepper1.run(3); // caution, delay under 3 is inaccurate. 
}

