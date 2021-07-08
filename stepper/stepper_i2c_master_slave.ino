//I2C setup
#include <Wire.h>
#define I2C_master false //define false if slave
#define I2C_adr 0
//
//#define interrupt_pin 2
//#define master_interrupt_pin 2
//Stepper setup
#define stepper_pul 3
#define stepper_dir 4
#define stepper_en 5

uint16_t pul_per_rev=12800;//set the pulse per revolution specified by the stepper driver here.
unsigned int us_between_pul=(double(360)/(double(pul_per_rev)*double(360)))*double(pow(10,6));
//
//data structure: [operation, value]
//operations: 0:enable/disable (value: 0-disable, 1-enable),1:set velocity for step mode(value: angle per second) 2:step mode(value: angle in degrees), 3:velocity mode(value: degrees per second)
#define enable_mode 0
#define set_velocity 1
#define step_mode 2
#define velocity_mode 3
#define direction_mode 4
//
//boolean executed=false;
//boolean interrupted=false;
volatile uint16_t data[2]={1,360};
volatile unsigned int last_spd=0;
unsigned int acc=1;
int data_l=sizeof(data)/sizeof(uint16_t);
void setup() {
  if(I2C_master){
    Wire.begin();
//    pinMode(master_interrupt_pin, OUTPUT);
//    digitalWrite(master_interrupt_pin, LOW);
  }
  else{
    Wire.begin(I2C_adr);
    Wire.onReceive(I2C_receive);
    //attachInterrupt(digitalPinToInterrupt(interrupt_pin), abc, RISING);
    //initialize stepper driver
    pinMode(stepper_pul, OUTPUT);
    pinMode(stepper_dir, OUTPUT);
    pinMode(stepper_en, OUTPUT);
    digitalWrite(stepper_pul, HIGH);
    digitalWrite(stepper_dir, HIGH);
    digitalWrite(stepper_en, HIGH);
    //
  }

  Serial.begin(115200);
//  Serial.setTimeout(10);
 Serial.println(us_between_pul);
  //pinMode(13, OUTPUT);
/*
  if(I2C_master){
    data[0]=3;
    unsigned long t=millis();
    while(1){
      int on99=1440*(sin(2*PI*0.5*(millis()-t)/1000));
      if(on99<0){
        data[0]=4;
        data[1]=0;
        I2C_send(0,data,2);
        on99=-on99;
      }
      else{
        data[0]=4;
        data[1]=1;
        I2C_send(0,data,2);
      }
      data[0]=3;
      data[1]=on99;
      Serial.println(data[1]);
      I2C_send(0,data,2);
      delay(10);
    }
  }
*/
}

void loop() {
  
  if(I2C_master){
    SerialFlush();
    data[0]=GetInt();
    Serial.println(data[0]);
    data[1]=GetInt();
    Serial.println(data[1]);
//    data[0]=0;
//    if(data[1]==1){
//      data[1]=0;
//      digitalWrite(13,LOW);
//    }
//    else{
//      data[1]=1;
//      digitalWrite(13,HIGH);
//    }
//    
    I2C_send(0,data,data_l);
  }
  else{
    switch(data[0]){
    case enable_mode:
//      Serial.println("en");
      if(data[1]==0){
        digitalWrite(stepper_en, LOW);
      }
      else if(data[1]==1){
        digitalWrite(stepper_en, HIGH);
      }
      last_spd=0;
    break;
    case set_velocity:
      stepper_set_velocity(data[1]);
      last_spd=0;
    break;
    case step_mode:
      stepper_run_step(data[1]);
    break;
    case velocity_mode:
      stepper_run_speed(data[1]);
    break;
    case direction_mode:
      stepper_set_direction(data[1]);
      last_spd=0;
    break;
    default:
    break;
    }
//    Serial.print(data[0]);
//    Serial.print(" ");
//    Serial.println(data[1]);
  } 
}

void I2C_send(int adr, uint16_t data_send[], int data_send_l){
//  digitalWrite(master_interrupt_pin, HIGH);
  Wire.beginTransmission(adr);
  for(int i=0;i<data_send_l;i++){
    for(int j=0;j<sizeof(uint16_t);j++){
      Wire.write((data_send[i] >> (j*8)) & 0xFF);
    }
  }
  Wire.endTransmission();
//  digitalWrite(master_interrupt_pin, LOW);
}

void I2C_receive(int num_of_byte){
  int data_receive_l=num_of_byte/sizeof(uint16_t);
  Serial.println("hi");
  uint16_t data_receive[data_receive_l];
  for(int i=0;i<data_receive_l;i++){
    for(int j=0;j<sizeof(uint16_t);j++){
      if(j==0){
        data_receive[i]=Wire.read();
      }
      else{
        data_receive[i]|=Wire.read() << (j*8);
      }
    }
  }

//  
  for(int i=0;i<data_l;i++){
    data[i]=data_receive[i];
  }
    Serial.print(data[0]);
    Serial.print(" ");
  Serial.println(data[1]);
//  executed=false;
//  interrupted=false;
}

void stepper_set_velocity(unsigned int dps){
  if(dps == 0){
    data[0]=0;
    data[1]=1;
  }
  else{
    us_between_pul = (double(360)/(double(pul_per_rev)*double(dps)))*double(pow(10,6));
    if(us_between_pul == 0){
      us_between_pul=1;
    }
  }
}

void stepper_run_step(int degree){
  data[0]=99;
  unsigned long steps=double(pul_per_rev)/double(360)*double(degree);
  unsigned int spd=double(us_between_pul)/double(2);
  last_spd=spd;
//  Serial.println(us_between_pul);
  unsigned long t_0 = micros();
  //unsigned long t_i=millis();
  //boolean pul = true;
  PORTD|=(uint8_t)1<<stepper_pul;
  unsigned long i=0;
  while(i<steps*2 && data[0]==99){
    if(micros()-t_0 >= spd){
      //pul^=1;
      PORTD^=(uint8_t)1<<stepper_pul;
      t_0=micros();   
      i+=1;
    }
  }
  if(i==steps*2){
    last_spd=0;
  }
//  Serial.println(millis()-t_i);
//  Serial.println(i);
//  Serial.print("timei:"); Serial.println(t_i);
//  Serial.print("time0:"); Serial.println(t_0);
  
//  data[0]=0;
//  data[1]=1;
}
void stepper_run_speed(uint16_t dps1){
  data[0]=99;
  //millis();                                 //???????????????????????????????????????????????????????????????????????????????????????????????????????
  unsigned long t_0 = micros();
  unsigned long t_1 = millis();
  //boolean pul = true;
  //digitalWrite(stepper_pul, pul);
  PORTD|=(uint8_t)1<<stepper_pul;
  stepper_set_velocity(dps1);
  unsigned int spd=double(us_between_pul)/double(2);
  if(last_spd==0){
    last_spd=100;
  }
  unsigned int temp_spd=last_spd;
  unsigned int d_spd=acc;
  /*
  while(data[0]==99 && temp_spd!=spd){
    if(micros()-t_0>=temp_spd){
      PORTD^=1<<stepper_pul;
      t_0=micros();   
    }
    if(millis()-t_1>=100){
      t_1=millis();
      if(temp_spd>spd){
        temp_spd-=d_spd;
        if(temp_spd<=spd){
         temp_spd=spd;
        }
      }
      else if(temp_spd<spd){
       temp_spd+=d_spd;
        if(temp_spd>=spd){
         temp_spd=spd;
        }
      }
      last_spd=temp_spd;
    }    
  }
  */
  while(data[0]==99){
    if(micros()-t_0 >= spd){
      //pul^=1;
      //digitalWrite(stepper_pul, pul);
      PORTD^=1<<stepper_pul;
      t_0=micros();   
    }
  }
}

void stepper_set_direction(unsigned int dir){
  if(dir==1){
    digitalWrite(stepper_dir, HIGH);
  }
  else if(dir==0){
    digitalWrite(stepper_dir, LOW);
  }
}

void abc(){
//  interrupted=true;
}

uint16_t GetInt(){
    while(Serial.available()==0){
  }
  uint16_t input = Serial.parseInt();
  SerialFlush();
  return input;
}
void SerialFlush(){
  while(Serial.available()){
    Serial.read();
  }
}
