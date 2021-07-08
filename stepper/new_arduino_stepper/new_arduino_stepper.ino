/* My self defined arduino stepper script

*/



void setup() {
  // put your setup code here, to run once:
    DDRD = DDRD | B11111100; //initialize pin 2 to 7 as output pin, leaving pin0, 1 (tx, rx)
    
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
    setOutputPins
    delayMicroseconds(5);
}

void setOutputPins(uint8_t pin, bool state){
    // set outputPin as HIGH or LOW.
    switch(pin)
    {
    case (pin >=0 && pin <= 7):
        if (state)
            PORTD |= (1 << pin);
        else
            PORTD &= ~(1 << pin);
        break;

    case (pin > 7 && pin pin < 14):
        pin -= 8
        if (state)
            PORTB |= (1 << pin);
        else
            PORTB &= ~(1 << pin);
        break;
    }
}
