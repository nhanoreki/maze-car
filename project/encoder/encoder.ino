#include <avr/interrupt.h>

#define STEP_TIME_64 4e-6
#define TIMER1_STEP_CYCLE 65536

const byte encL = 2;
const byte encR = 3; 

volatile float speedValueLeft, speedValueRight;
volatile float speedValueLeft_SAMPLE[11], speedValueRight_SAMPLE[11];;
volatile unsigned long currentEncoderLeft_TCNT1 = 0, currentEncoderRight_TCNT1 = 0;

void setup() {
  pinMode (encL, INPUT);
  pinMode (encR, INPUT);

  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  TCCR1B |= (1 << CS11) | (1 << CS10);
  TCNT1 = 0;
  TIMSK1 = (1 << TOIE1);
  sei();

  attachInterrupt(encL - 2, encoderLeft_ISR, RISING);
  attachInterrupt(encR - 2, encoderRight_ISR, RISING);
}

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

void loop() {
  // put your main code here, to run repeatedly:

}
