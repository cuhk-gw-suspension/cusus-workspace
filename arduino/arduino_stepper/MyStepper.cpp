#include "MyStepper.h"

Stepper::Stepper(uint8_t pul_pin, uint8_t dir_pin){
    _pul_pin = pul_pin;
    _dir_pin = dir_pin;
    
    _currentPos = 0;
    _direction = 0;

    _step_interval = 100; // us

    initOutputPins(_pul_pin);
    initOutputPins(_dir_pin);

    setOutputPins(pul_pin, LOW);
}

void Stepper::sweep(uint8_t pin1, uint8_t pin2){
    pinMode(pin1, INPUT_PULLUP);
    pinMode(pin2, INPUT_PULLUP);

    setDirection(LOW); 
    while(readPin(pin1) && readPin(pin2)){
        delayMicroseconds(_step_interval);
        step(3);
    }
    setPosition(0); // set one boundary as 0 position.
    delay(500);
    uint8_t *otherPin;

    if (readPin(pin1))
        otherPin = &pin1;
    else if (readPin(pin2))
        otherPin = &pin2;

    setDirection(HIGH);
    _max_dist_from_0 = 0;
    while(readPin(*otherPin)){
        delayMicroseconds(_step_interval);
        step(3);
        _max_dist_from_0 += 1;
    }
    _max_dist_from_0 /= 2;
    
    setPosition(_max_dist_from_0); // set centre as 0 position.
    moveTo(0);
    while (distanceToGo() != 0){
        if (readPin(pin1) && readPin(pin2)){
            /* delayMicroseconds(_step_interval); */
            run(3);
        }
    }

    _bound_set = true;
}

void Stepper::moveTo(long absolute){
    _targetPos = absolute;
    long displacement = _targetPos - _currentPos;
    if (displacement > 0)
        setDirection(HIGH);
    else if (displacement < 0)
        setDirection(LOW);
}

void Stepper::setDirection(bool direction){
    _direction = direction;
    setOutputPins(_dir_pin, _direction);
}

void Stepper::setSpeed(unsigned int speed){
    float period = 1e6/speed;
    _step_interval = (unsigned int)period;
}

void Stepper::run(unsigned int pulsewidth){
    if (_currentPos != _targetPos){
        if (_bound_set && abs(_currentPos) <= _max_dist_from_0)
            return;

        step(pulsewidth);
        _currentPos += _direction ? 1 : -1;
        delayMicroseconds(_step_interval);
    }
}

void Stepper::step(unsigned int pulsewidth){
    setOutputPins(2, HIGH);
    
    // need to replace by other delay method
    // claims to be accurate 3 us and up
    delayMicroseconds(pulsewidth); 
    
    setOutputPins(2, LOW);
}

long Stepper::getPosition(){
    return _currentPos;
}

long Stepper::distanceToGo(){
    return _targetPos - _currentPos;
}

void Stepper::setPosition(long pos){
    _currentPos = pos; 
}

char Stepper::locatePinSector(uint8_t pin){ 
    if (pin < 8)
        return 'D';
    else if (pin < 14)
        return 'B';
}

void Stepper::initOutputPins(uint8_t pin) {
    volatile uint8_t *addrToDataDirectionRegister;
    switch(locatePinSector(pin))
    {
    case 'D':
        addrToDataDirectionRegister = &DDRD;
        break;

    case 'B':
        pin -= 8;
        addrToDataDirectionRegister = &DDRB;
        break;
    }
    
    *addrToDataDirectionRegister |= (1 << pin); // initialize pin 
}

void Stepper::setOutputPins(uint8_t pin, bool state){
    // set outputPin as HIGH or LOW.
    volatile uint8_t *addrToDataRegister;

    switch(locatePinSector(pin))
    {
    case 'D':
        addrToDataRegister = &PORTD;
        break;

    case 'B':
        pin -= 8;
        addrToDataRegister = &PORTB;
        break;
    }

    if (state)
        *addrToDataRegister |= (1 << pin); // wrtie HIGH to the pin
    else
        *addrToDataRegister &= ~(1 << pin); // write LOW to the pin
}


bool Stepper::readPin(uint8_t pin){
    char sector;
    if (pin < 8)  sector = 'D';
    else if (pin < 14)  sector = 'B';

    switch (sector)
    {
    case 'D':
        return ((PIND & (1 << pin)) >> pin);
    case 'B':
        pin -= 8;
        return ((PINB & (1 << pin)) >> pin);
    }
}

