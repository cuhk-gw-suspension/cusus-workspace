#ifndef MyStpper_h
    #define MyStpper_h

#include <stdlib.h>
#if ARDUINO >= 100
    #include <Arduino.h>
#endif

// This defs cause troubles on some version of Arduino
#undef rounds

// copy from other, dont know why
#if (defined(ARDUINO) && ARDUINO >= 155) || defined(ESP8266)
    #define YIELD yield();
#else
    #define YIELD;
#endif


class Stepper{
public:
    //default Stepper object, 
    Stepper(uint8_t pul_pin = 2, uint8_t dir_pin = 3, bool enable = true);
     
    // setting the target position in terms of absolute position
    void moveTo(long absolute);
    
    // set the direction of stepper moving, true=HIGH, false=LOW
    void setDirection(bool direction);

    // function to call in the arduino loop
    void run();

    // move one step
    void step();

    // return current absolute position
    inline long getPosition();

    inline long distanceToGo();

protected:
    // current position in terms of absolute position
    long _currentPos;

    // target position in terms of absolute position
    long _targetPos;

    uint8_t _pul_pin;
    uint8_t _dir_pin;
    bool _enable;

    // current direction, true = HIGH, false = LOW
    bool _direction;
    
    // locate which data register sector the pin is on. 'B' or 'D'.
    char locatePinSector(uint8_t pin);
    
    // initialize provide pin as output
    void initOutputPins(uint8_t pin);
    
    // set provide pin as HIGH or LOW
    void setOutputPins(uint8_t pin, bool state);
    
}


#endif
