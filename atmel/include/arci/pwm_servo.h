#ifndef _PWM_SERVO_H
#define _PWM_SERVO_H

#define CLOCK_MHZ 8000000
#define PRESCALER 64
#define TIMER_CLK 125000
#define TIMER_PERIOD 8

void move_servo(double frequency, short high);
void move_motor(double frequency, short high);
void move_motor_reverse(double frequency, short high);
void servo_on();
void motor_on();
void motor_off();
void servo_off();

#endif  // PWM_SERVO_H
