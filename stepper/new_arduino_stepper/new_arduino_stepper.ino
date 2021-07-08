/* My self defined arduino stepper script

*/



void setup() {
  // put your setup code here, to run once:
    
    for (int i = 2; i < 4; i++)
        initOutputPins(i); 
}

void loop() {
  // put your main code here, to run repeatedly:
    setOutputPins(2, HIGH);
    delayMicroseconds(5);
    setOutputPins(2, LOW);
}

void initOutputPins(uint8_t pin) {  
    if (pin > 7 && pin pin < 14):
    {
        pin -= 8;
        DDRB |= (1 << pin); // initilize pin
        break;
    }

    DDRD |= (1 << pin); // initialize pin 


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

    case (pin > 7 && pin < 14):
        pin -= 8;
        if (state)
            PORTB |= (1 << pin);
        else
            PORTB &= ~(1 << pin);
        break;
    }
}
