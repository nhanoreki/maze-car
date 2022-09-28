#include <avr/interrupt.h>

#define turnLeft 0
#define turnRight 1
#define noTurn -1
#define goAhead 1
#define goBack 0
#define standHere -1
#define left 0
#define right 1
#define MIN_PWM 0
#define MAX_PWM 255

const int IN1 = 4;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;
const int enc[2] = {2, 3};

volatile signed long pre = 0;
volatile float speedValue;
volatile int setSpeedPWM = 0;
volatile float sumSpeedValue = 0;
volatile float arrSpeedValue[10];

int index = 0;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(enc[left], INPUT);
//  pinMode(enc[right], INPUT);

  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  TCCR1B |= (1 << CS11) | (1 << CS10);
  TIMSK1 |= (1 << TOIE1);
  TCNT1 = 0;
  sei();

  attachInterrupt(enc[left] - 2, counterPulseLeft, RISING);
//  attachInterrupt(enc[right] - 2, counterPulseRight, RISING);
}

ISR(TIMER1_OVF_vect) {
  TCNT1 = 0;
}

void counterPulseLeft() {
  speedValue = 250000.0 / (((signed long)TCNT1 + 65536 - pre) % 65536);
  sumSpeedValue += -arrSpeedValue[index] + speedValue;
  arrSpeedValue[index] = speedValue;
  index = (index + 1) % 10;
  pre = TCNT1;
}

void motorSetup(int carMovement, int carTurn, int speedPWM) {
  speedPWM = constrain(speedPWM, MIN_PWM, MAX_PWM);

  if (carMovement == goAhead) {
    digitalWrite(IN1, goAhead);
    digitalWrite(IN4, goAhead);
    if (carTurn == turnLeft) {
      analogWrite(IN2, MAX_PWM - speedPWM);
      analogWrite(IN3, MAX_PWM - speedPWM / 2);
    } else if (carTurn == turnRight) {
      analogWrite(IN2, MAX_PWM - speedPWM / 2);
      analogWrite(IN3, MAX_PWM - speedPWM);
    } else {
      analogWrite(IN2, MAX_PWM - speedPWM);
      analogWrite(IN3, MAX_PWM - speedPWM);
    }
  } else if (carMovement == goBack) {
    digitalWrite(IN1, goBack);
    digitalWrite(IN4, goBack);
    if (carTurn == turnLeft) {
      analogWrite(IN2, speedPWM);
      analogWrite(IN3, speedPWM / 2);
    } else if (carTurn == turnRight) {
      analogWrite(IN2, speedPWM / 2);
      analogWrite(IN3, speedPWM);
    } else {
      analogWrite(IN2, speedPWM);
      analogWrite(IN3, speedPWM);
    }
  } else {
    if (carTurn == turnLeft) {
      digitalWrite(IN1, goAhead);
      digitalWrite(IN4, goBack);
      analogWrite(IN2, MAX_PWM - speedPWM);
      analogWrite(IN3, speedPWM);
    } else if (carTurn == turnRight) {
      digitalWrite(IN1, goBack);
      digitalWrite(IN4, goAhead);
      analogWrite(IN2, speedPWM);
      analogWrite(IN3, MAX_PWM - speedPWM);
    } else {
      digitalWrite(IN1, 0);
      digitalWrite(IN4, 0);
      analogWrite(IN2, 0);
      analogWrite(IN3, 0);
    }
  }
}

void loop() {
//  motorSetup(goAhead, noTurn, 255);
//  Serial.println(sumSpeedValue / 10);
Serial.println(sizeof(long long));
}
