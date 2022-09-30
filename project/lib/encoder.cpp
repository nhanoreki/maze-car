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