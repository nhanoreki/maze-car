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

const bool enc_Status = LOW;
const bool echo_Status = LOW;

struct pairCounterPulse {
  bool Status = false;
  int Value = 0;
};

volatile double pulsesPerSecondLeft, pulsesPerSecondRight, distanceHead = 50, distanceLeft = 50, distanceRight = 50;
volatile unsigned long timerPoint = 0, currentEncoderLeft = 0, currentEncoderRight = 0, currentSensorHead = 0, currentSensorLeft = 0, currentSensorRight = 0;
volatile int allSensor = 0;
volatile bool headHigh = 0, leftHigh = 0, rightHigh = 0;
volatile bool headIsRec = 0;
volatile pairCounterPulse counterPulse;

int error = 0;
int setSpeedPWM = 255;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(encL, INPUT);
  pinMode(encR, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echoH, INPUT_PULLUP);
  pinMode(echoL, INPUT_PULLUP);
  pinMode(echoR, INPUT_PULLUP);
  
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
  attachInterrupt(0, enc_left_isr, RISING);
  attachInterrupt(1, enc_right_isr, RISING);
  
  
  digitalWrite(trig, HIGH);
}

ISR (TIMER1_OVF_vect) {
  TCNT1 = 0;
}

void enc_left_isr() {
  pulsesPerSecondLeft = 1 / (((TCNT1 - currentEncoderLeft + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE) * STEP_TIME_64);
  currentEncoderLeft = TCNT1;
  if (counterPulse.Status) {
    counterPulse.Value++;
  }
}

void enc_right_isr() {
  pulsesPerSecondRight = 1 / (((TCNT1 - currentEncoderRight + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE) * STEP_TIME_64);
  currentEncoderRight = TCNT1;
}

ISR (PCINT0_vect) {
  if (digitalRead(echoR) != rightHigh) {
    if (rightHigh) {
      distanceRight = SOUND_SPEED * (((TCNT1 - currentSensorRight + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE) * STEP_TIME_64) / 2;
      ++allSensor;
    } else {
      currentSensorRight = TCNT1; 
    }
    rightHigh = !rightHigh;
  }
  if (digitalRead(echoL) != leftHigh) {
    if (leftHigh) {
      distanceLeft = SOUND_SPEED * (((TCNT1 - currentSensorLeft + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE) * STEP_TIME_64) / 2;
      ++allSensor;
    } else {
      currentSensorLeft = TCNT1; 
    }
    leftHigh = !leftHigh;
  }
  if (digitalRead(echoH) != headHigh) 
  {
    if (headHigh){
      distanceHead = SOUND_SPEED * (((TCNT1 - currentSensorHead + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE) * STEP_TIME_64) / 2;
      ++allSensor;
    } else
    {
      currentSensorHead = TCNT1;
    }
    headHigh = !headHigh;
  }

  if (allSensor == 3) {
    allSensor = 0;
    digitalWrite(trig, HIGH);
  }
}

void motorRight(int speed_pwm, bool x) {
  speed_pwm = constrain(speed_pwm, MIN_PWM, MAX_PWM);
  digitalWrite(IN1, x);
  analogWrite(IN2, abs(255*x - speed_pwm));
}

void motorLeft(int speed_pwm, bool x) {
  speed_pwm = constrain(speed_pwm, MIN_PWM, MAX_PWM);
  digitalWrite(IN4, x);
  analogWrite(IN3, abs(255*x - speed_pwm));
}

void goAhead() {
  motorLeft(setSpeedPWM, 1);
  motorRight(setSpeedPWM, 1);
}

void goBack() {
  motorLeft(setSpeedPWM, 0);
  motorRight(setSpeedPWM, 0);
}

void stopCar() {
  motorLeft(0, 0);
  motorRight(0, 0);
  stopCar();
}

void turnLeft() {
  motorLeft(setSpeedPWM, 0);
  motorRight(setSpeedPWM, 1);
}

void turnRight() {
  motorLeft(setSpeedPWM, 1);
  motorRight(setSpeedPWM, 0);
}

void loop() {
  if (distanceHead <30) {
    error = 0;
    if (counterPulse.Status == false) {
      counterPulse.Status = true;
      if (distanceLeft < 30) {
        turnRight();
      } else {
        turnLeft();
      }
    }
  } else if (counterPulse.Status == false) {
    goAhead();
  }

//  if (distanceHead >= 1000) {
//    error++;
//    if (error > 10) {
//      counterPulse.Status = true;
//      goBack();  
//    }
//    if (error > 50) {
//      stopCar();
//    }
//  }
  
  if (counterPulse.Value > 12) {
    setSpeedPWM = 255;
    counterPulse.Value = 0;
    counterPulse.Status = false;
  }

  if (counterPulse.Status == false) {
//    if (distanceHead < 80 && setSpeedPWM > 150) {
//      setSpeedPWM--;
//    }
    if (distanceLeft < 15) {
      motorLeft(255, 1);
      motorRight(175, 1);
    } else if (distanceRight < 15) {
      motorLeft(175, 1);
      motorRight(255, 1);
    }
  }
  
  digitalWrite(trig, LOW);
  Serial.print(distanceHead);
  Serial.print(" ");
  Serial.print(counterPulse.Status*100);
  Serial.print(" ");
  Serial.println(setSpeedPWM);
//  Serial.print(distanceLeft);
//  Serial.print(" ");
//  Serial.print(distanceRight);
//  Serial.print(" ");
//  Serial.print(error * 10);
//  Serial.print(" ");
//  Serial.print(counterPulse.Status*1000);
//  Serial.print(" ");
//  Serial.println(counterPulse.Value*100);
}
