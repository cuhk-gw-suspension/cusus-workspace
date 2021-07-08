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
    if (pin > 7 && pin < 14)
    {
        pin -= 8;
        DDRB |= (1 << pin); // initilize pin
    }
    else
        DDRD |= (1 << pin); // initialize pin 


}

void setOutputPins(uint8_t pin, bool state){
    // set outputPin as HIGH or LOW.
    char _data_sector;
    if (pin < 8)
        _data_sector = 'D';
    if (pin < 14)
        _data_sector = 'B';
    switch(_data_sector)
    {
    case 'D':
        if (state)
            PORTD |= (1 << pin);
        else
            PORTD &= ~(1 << pin);
        break;

    case 'B':
        pin -= 8;
        if (state)
            PORTB |= (1 << pin);
        else
            PORTB &= ~(1 << pin);
        break;
    }
}
