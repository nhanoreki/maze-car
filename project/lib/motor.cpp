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