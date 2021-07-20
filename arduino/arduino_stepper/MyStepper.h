#ifndef MyStepper_h
    #define MyStepper_h

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


// read the Pin state, HIGH or LOW.
bool readPin(uint8_t pin);

class Stepper{
public:
    //default Stepper object, 
    Stepper(uint8_t pul_pin = 2, uint8_t dir_pin = 3);
     
    // sweep the track and centre the table. 
    // pin1, pin2 are pin of the limit switches at boundaries.
    // LOW indicates trigger of the switch. (PULLUP mode)
    void sweep(uint8_t pin1, uint8_t pin2);

    // setting the target position in terms of absolute position
    void moveTo(long absolute);
    
    // set the direction of stepper moving, true=HIGH, false=LOW
    void setDirection(bool direction);

    // set number of steps stepper take per sec.
    void setSpeed(unsigned int);
    
    // set pulse width, in microseconds.
    // default to 3 us.
    /* void setPulseWidth(unsigned int); */

    // function to call in the arduino loop
    void run();

    // move one step
    void step();

    // return current absolute position
    long getPosition();

    // return displacement between current position and target position
    long distanceToGo();

    // set current position as pos in absolute position
    void setPosition(long pos);

private:
    // locate which data register sector the pin is on. 'B' or 'D'.
    char locatePinSector(uint8_t pin);
    
    // initialize provide pin as output
    void initOutputPins(uint8_t pin);
    
    // set provide pin as HIGH or LOW
    void setOutputPins(uint8_t pin, bool state);
    
protected:
    // current position in terms of absolute position
    volatile long _currentPos;

    // target position in terms of absolute position
    volatile long _targetPos;

    long _max_dist_from_0;
    bool _bound_set = false;
    
    // time to wait between LOW and HIGH, in microsec.
    unsigned int _step_interval;

    // time to wait between HIGH and LOW, in microsec.
    const unsigned int _pulse_width = 3;

    uint8_t _pul_pin;
    uint8_t _dir_pin;

    // current direction, true = HIGH, false = LOW
    bool _direction;
    
};


#endif
