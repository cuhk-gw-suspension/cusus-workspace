#include "MyStepper.h"

Stepper::Stepper(uint8_t pul_pin, uint8_t dir_pin, bool enable){
    _pul_pin = pul_pin;
    _dir_pin = dir_pin;
    _enable = enable;
    
    _currentPos = 0;
    _direction = 0;

    initOutputPins(_pul_pin);
    initOutputPins(_dir_pin);
}

void Stepper::moveTo(long absolute){
    _targetPos = absolute;
    int displacement = _targetPos - _currentPos;
    if (displacement > 0)
        setDirection(HIGH);
    else if (displacement < 0)
        setDirection(LOW);
}

void Stepper::setDirection(bool direction){
    _direction = direction;
    setOutputPins(_dir_pin, _direction);
}

void Stepper::run(){
    if (_currentPos != _targetPos){
        step();
        _currentPos += _direction ? 1 : -1;
    }
}

void Stepper::step(){
    setOutputPins(2, HIGH);
    delayMicroseconds(2); // need to replace by other delay method
    setOutputPins(2, LOW);
}

long Stepper::getPosition(){
    return _currentPos;
}

long Stepper::distanceToGo(){
    return _targetPos - _currentPos;
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



