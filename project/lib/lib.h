#pragma once

#include <Cinit.h>

#include <encoder.h>

ISR (TIMER1_OVF_vect) {
  TCNT1 = 0;
}

void encoderLeft_ISR() {
  static byte i = 0;
  speedValueLeft_SAMPLE[10] -= speedValueLeft_SAMPLE[i];
  speedValueLeft_SAMPLE[i] = (TCNT1 - currentEncoderLeft_TCNT1 + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE;
  speedValueLeft_SAMPLE[10] += speedValueLeft_SAMPLE[i];
  speedValueLeft = 250000 / (speedValueLeft_SAMPLE[10] / 10);
  i = (i + 1) % 10;
  currentEncoderLeft_TCNT1 = TCNT1;
}

void encoderRight_ISR() {
  static byte i = 0;
  speedValueRight_SAMPLE[10] -= speedValueRight_SAMPLE[i];
  speedValueRight_SAMPLE[i] = (TCNT1 - currentEncoderRight_TCNT1 + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE;
  speedValueRight_SAMPLE[10] += speedValueRight_SAMPLE[i];
  speedValueRight = 250000 / (speedValueRight_SAMPLE[10] / 10);
  i = (i + 1) % 10;
  currentEncoderRight_TCNT1 = TCNT1;
}

#include <motor.h>

void motorLeft(byte speed_pwm, bool x) {
  speed_pwm = constrain(speed_pwm, MIN_PWM, MAX_PWM);
  digitalWrite(IN4, x);
  analogWrite(IN3, abs(255*x - speed_pwm));
}

void motorRight(byte speed_pwm, bool x) {
  speed_pwm = constrain(speed_pwm, MIN_PWM, MAX_PWM);
  digitalWrite(IN1, x);
  analogWrite(IN2, abs(255*x - speed_pwm));
}

#include <sensor.h>

ISR (TIMER1_OVF_vect) {
  TCNT1 = 0;
}

ISR (PCINT0_vect) {
  if (digitalRead(echoH) != headHigh) {
    if (headHigh){
      static byte i = 0;
      distanceHead_SAMPLE[10] -= distanceHead_SAMPLE[i];
      distanceHead_SAMPLE[i] = (TCNT1 - currentSensorHead + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE;
      distanceHead_SAMPLE[10] += distanceHead_SAMPLE[i];
      distanceHead = ((SOUND_SPEED * STEP_TIME_64) * (distanceHead_SAMPLE[10] / 10)) / 2;
      i = (i + 1) % 10;
      ++allSensor;
    } else {
      currentSensorHead = TCNT1;
    }
    headHigh = !headHigh;
  }
  if (digitalRead(echoL) != leftHigh) {
    if (leftHigh) {
      static byte i = 0;
      distanceLeft_SAMPLE[10] -= distanceLeft_SAMPLE[i];
      distanceLeft_SAMPLE[i] = (TCNT1 - currentSensorLeft + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE;
      distanceLeft_SAMPLE[10] += distanceLeft_SAMPLE[i];
      distanceLeft = ((SOUND_SPEED * STEP_TIME_64) * (distanceLeft_SAMPLE[10] / 10)) / 2;
      i = (i + 1) % 10;
      ++allSensor;
    } else {
      currentSensorLeft = TCNT1; 
    }
    leftHigh = !leftHigh;
  }
  if (digitalRead(echoR) != rightHigh) {
    if (rightHigh) {
      static byte i = 0;
      distanceRight_SAMPLE[10] -= distanceRight_SAMPLE[i];
      distanceRight_SAMPLE[i] = (TCNT1 - currentSensorRight + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE;
      distanceRight_SAMPLE[10] += distanceRight_SAMPLE[i];
      distanceRight = ((SOUND_SPEED * STEP_TIME_64) * (distanceRight_SAMPLE[10] / 10)) / 2;
      i = (i + 1) % 10;
      ++allSensor;
    } else {
      currentSensorRight = TCNT1; 
    }
    rightHigh = !rightHigh;
  }
  if (allSensor == 3) {
    allSensor = 0;
    digitalWrite(trig, HIGH);
  }
}