#pragma once

#include <avr/interrupt.h>

#define SOUND_SPEED 34320
#define MAX_PWM 255
#define MIN_PWM 0
#define STEP_TIME_64 4e-6
#define TIMER1_STEP_CYCLE 65536

const int IN1 = 4;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;
const int encL = 2;
const int encR = 3;
const int trig = 8;
const int echoH = 9;
const int echoL = 10;
const int echoR = 11;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(encL, INPUT);
  pinMode(encR, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echoH, INPUT);
  pinMode(echoL, INPUT);
  pinMode(echoR, INPUT);
  
  cli();
  // Timer/Counter
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;

  TCCR1B |= (1 << CS11) | (1 << CS10);
  TCNT1 = 0;
  TIMSK1 = (1 << TOIE1);

  // Pin Change Interrupt
  PCICR = 0;
  PCMSK0 = 0;

  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3);

  sei();
  
  // External Interrupt
  attachInterrupt(encL - 2, encoderLeft_ISR, RISING);
  attachInterrupt(encR - 2, encoderRight_ISR, RISING);
  
  digitalWrite(trig, HIGH);
}