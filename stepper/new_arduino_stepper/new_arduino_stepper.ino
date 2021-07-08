/* My self defined arduino stepper script

*/

int step_per_rev = 25600;
int count = 0;

void setup() {
  // put your setup code here, to run once:
    
    for (int i = 2; i < 4; i++)
        initOutputPins(i); 
}

void loop() {
  // put your main code here, to run repeatedly:
}

void moveTo(){

}

bool run(){
    setOutputPins(2, HIGH);
    delayMicroseconds(5); // need to replace by other delay method
    setOutputPins(2, LOW);
}

char _locate_pin_sector(uint8_t pin){ 
    if (pin < 8)
        return 'D';
    else if (pin < 14)
        return 'B';

    return false;
}

void initOutputPins(uint8_t pin) {
    volatile uint8_t *addrToDataDirectionRegister;
    switch(_locate_pin_sector(pin))
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

void setOutputPins(uint8_t pin, bool state){
    // set outputPin as HIGH or LOW.
    volatile uint8_t *addrToDataRegister;

    switch(_locate_pin_sector(pin))
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
        *addrToDataRegister |= (1 << pin);
    else
        *addrToDataRegister &= ~(1 << pin);
}
