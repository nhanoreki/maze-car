#define MAX_PWM 255
#define MIN_PWM 0

const byte IN1 = 4;
const byte IN2 = 5;
const byte IN3 = 6;
const byte IN4 = 7;

void setup() {
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
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

void loop() {
  
}
