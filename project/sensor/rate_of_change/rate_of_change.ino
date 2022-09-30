#include <avr/interrupt.h>

#define SOUND_SPEED 34320
#define STEP_TIME_64 4e-6
#define TIMER1_STEP_CYCLE 65536

const byte trig = 8;
const byte echoH = 9;
const byte echoL = 10;
const byte echoR = 11;


volatile float distanceHead, distanceLeft, distanceRight;
volatile float rateOfChangeDistanceHead, lastDistanceHead = 0;
volatile float distanceHead_SAMPLE[11], distanceLeft_SAMPLE[11], distanceRight_SAMPLE[11];
volatile float rateOfChangeDistanceHead_SAMPLE[11];
volatile unsigned long currentSensorHead = 0, currentSensorLeft = 0, currentSensorRight = 0;
volatile byte allSensor = 0;
volatile bool headHigh = false, leftHigh = false, rightHigh = false;
volatile bool headIsRec = false;

void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echoH, INPUT);
  pinMode(echoL, INPUT);
  pinMode(echoR, INPUT);
  
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  TCCR1B |= (1 << CS11) | (1 << CS10);
  TCNT1 = 0;
  TIMSK1 = (1 << TOIE1);
  PCICR = 0;
  PCMSK0 = 0;
  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3);
  sei();
  
  digitalWrite(trig, HIGH);
}

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
      rateOfChangeDistanceHead_SAMPLE[10] -= rateOfChangeDistanceHead_SAMPLE[i];
      rateOfChangeDistanceHead_SAMPLE[i] = distanceHead - lastDistanceHead;
      rateOfChangeDistanceHead_SAMPLE[10] += rateOfChangeDistanceHead_SAMPLE[i];
      rateOfChangeDistanceHead = rateOfChangeDistanceHead_SAMPLE[10];
      lastDistanceHead = distanceHead;
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

void loop() {
  digitalWrite(trig, LOW);
  Serial.print(distanceHead);
  Serial.print(" ");
  Serial.print(0);
  Serial.print(" ");
  Serial.println(rateOfChangeDistanceHead);
}
