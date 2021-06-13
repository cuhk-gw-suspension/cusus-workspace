/*
  Arduino LSM9DS1 - Accelerometer Application

  This example reads the acceleration values as relative direction and degrees,
  from the LSM9DS1 sensor and prints them to the Serial Monitor or Serial Plotter.

  The circuit:
  - Arduino Nano 33 BLE

  Created by Riccardo Rizzo

  Modified by Jose García
  27 Nov 2020

  This example code is in the public domain.
*/

#include "Arduino_LSM9DS1_mod.h"

float x, y, z;
int degreesX = 0;
int degreesY = 0;
int degreesZ = 0;
unsigned long NewTime, OldTime, exact;

void setup() {
  Serial.begin(2000000);

  if (!IMU.begin(0)) // 0:2g, 1:4g, 2:8g, 3:16g, at 952Hz
  { 
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  
}

void loop() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z); //output x, y, z acceleration in terms of g.
//    exact = micros();
//    Serial.print(exact);
//    Serial.print(',');
    Serial.print(x,8);
    Serial.print(',');
    Serial.print(y,8);
    Serial.print(',');
    Serial.println(z,8);
  }
}
