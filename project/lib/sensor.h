#pragma once

#include <Cinit.h>

volatile float distanceHead, distanceLeft, distanceRight;
volatile float distanceHead_SAMPLE[11], distanceLeft_SAMPLE[11], distanceRight_SAMPLE[11];
volatile unsigned long currentSensorHead = 0, currentSensorLeft = 0, currentSensorRight = 0;
volatile byte allSensor = 0;
volatile bool headHigh = false, leftHigh = false, rightHigh = false;
volatile bool headIsRec = false;

ISR (TIMER1_OVF_vect);
ISR (PCINT0_vect);


