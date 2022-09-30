#pragma once

#include <Cinit.h>

volatile float speedValueLeft, speedValueRight;
volatile float speedValueLeft_SAMPLE[11], speedValueRight_SAMPLE[11];;
volatile unsigned long currentEncoderLeft_TCNT1 = 0, currentEncoderRight_TCNT1 = 0;

ISR (TIMER_OVF_vect);
void encoderLeft_ISR();
void encoderRight_ISR();