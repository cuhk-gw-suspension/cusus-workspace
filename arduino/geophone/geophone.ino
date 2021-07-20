const int analogPin = A4;
const int cs = 12;
const int clk = 2;
const int data_in = 4;
int i;

unsigned int val;
unsigned long time_new, time_old;

void setup() { 
  // put your setup code here, to run once:
  pinMode(cs, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(data_in, INPUT);
  digitalWrite(cs, HIGH);
  //delayMicroseconds(1);
  digitalWrite(clk, LOW);
  //delayMicroseconds(1);

  Serial.begin(500000);
  while (!Serial) { ;} // wait for connection

  time_old = micros();
}

void loop() {
  // put your main code here, to run repeatedly:
  time_new = micros();
  if (time_new - time_old >= 5000) {
//      val = analogRead(analogPin);
      val = ADC_val();
      Serial.println(val);
      time_old = time_new;
    }
}

void setOutputPins(uint8_t pin, bool state){
  // set the pin as HIGH or LOW
  volatile uint8_t *addrToDataRegister;

  if (pin < 8)
    addrToDataRegister = &PORTD;
  else if (pin < 14){
    pin -= 8;
    addrToDataRegister = &PORTB;
  }

  if (state)
    *addrToDataRegister |= (1 << pin); // write HIGH to pin
  else
    *addrToDataRegister &= ~(1 << pin);
}

unsigned int ADC_val(){
  bool data[12]={0,0,0,0,0,0,0,0,0,0,0,0};
  //bool data_test[12]={1,0,0,0,0,0,0,0,0,0,0,1};
  uint16_t val=0;
  setOutputPins(cs, LOW); //Pull chip select down to activate chip
  //delayMicroseconds(1); //delay at least 100ns T_SUCS. Maybe not needed.
  //ADC starts sampling;
  setOutputPins(clk, HIGH);
  //delayMicroseconds(1); //delay at least 312ns T_HI. Maybe not needed.
  setOutputPins(clk, LOW);
  //delayMicroseconds(1); //delay at least 312ns T_LO. Maybe not needed.
  setOutputPins(clk, HIGH);
  //delayMicroseconds(1); //delay at least 312ns T_HI. Maybe not needed.
  setOutputPins(clk, LOW);
  //delayMicroseconds(1); //delay at least 312ns T_LO. Maybe not needed.
  //delayMicroseconds(1); //delay at least 200ns T_en. Maybe not needed.
  //ADC finish sampling;
  //Serial output begins;
  setOutputPins(clk, HIGH);  //Dumps the null bit (see data sheet)
  //delayMicroseconds(1); //delay at least 312ns T_HI. Maybe not needed.
  setOutputPins(clk, LOW); //Latches the first bit. MSB first.
  //delayMicroseconds(1); //delay at least 312ns T_LO. Maybe not needed.
  //delayMicroseconds(1); //delay at least 200ns T_DO. Maybe not needed.
  for(i=11;i>=0;i--){ 
    setOutputPins(clk, HIGH);
    //delayMicroseconds(1); //delay at least 312ns T_HI. Maybe not needed.
    data[i]=digitalRead(data_in);
    setOutputPins(clk, LOW);
    //delayMicroseconds(1); //delay at least 312ns T_LO. Maybe not needed.
  }
  setOutputPins(cs, HIGH); //Pull chip select up to deactivate chip
  //delayMicroseconds(1); //delay at least 625ns T_CSH. Maybe not needed.
  //Calculating the ADC value
  for(i=11;i>=0;i--){
    val=val+int(0.5+data[i]*pow(2,i));
  }
  return(val);
}
