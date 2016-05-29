/*	jgome043_lab7_part1.c - 4/20/2016 3:37:08 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #7  Exercise #1
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#include <ucr/pwm_servo.c>

enum States { START, INIT, WAIT, OFF } state;      // state vars
unsigned char input_A;                  // temp var for PINA
unsigned char input_B;                  // temp var for PINA
unsigned char play;                     // var to hold last play status 
double freq = 50.00;
short right = 2000;
short left = 1000;
short middle = 1500;
short low = 20000;
short high = 1000;
short pos;

void Tick() {
  input_A = ~PINB;
  input_A &= 0x0F;
  input_B = ~PINA >> 4;
  input_B &= 0x0F;
  // State Transitions
  switch (state) {
    case START:
      state = INIT; break;        // transition to INIT state
    case INIT:
      state = WAIT; break;        // transition to WAIT state
    case WAIT: 
      // check if more than one button is pressed
      if ((input_A & 0x01 && input_A & 0x02) || 
          (input_A & 0x01 && input_A & 0x04) || 
          (input_A & 0x02 && input_A & 0x04)) {
        state = OFF;
      }
      // check if no button is pressed
      else if (input_A ^ 0x01 && input_A ^ 0x02 && input_A ^ 0x04) {
        // check if playing sound
        if (play)
          state = OFF;
      }
      break;
    case OFF:
      // check if no button is pressed
      if (input_A ^ 0x01 && input_A ^ 0x02 && input_A ^ 0x04) {
        state = WAIT;
      }
      break;
    default:
      state = START;              // error likely happened, restart
      break;
  }

  // State Actions
  switch (state) {
    case INIT:
      break;    // initialize temp var
    case WAIT:
      // check if button 1 is pressed
      if (input_A & 0x01) {
        //pos = (pos > left) ? pos-- : pos;
        pos = right;
        move_servo(freq, pos);
        play = 1;
      }
      // check if button 2 is pressed
      else if (input_A & 0x02) {
        //pos = (pos < left) ? pos++ : pos;
        pos = left;
        move_servo(freq, pos);
        play = 1;
      }
      // check if button 3 is pressed
      else if (input_A & 0x04) {
        pos = middle;
        move_servo(freq, pos);
        play = 1;
      }
      else if (input_B & 0x01) {
        //pos = (pos > left) ? pos-- : pos;
        pos = low;
        move_motor(freq, pos);
        play = 1;
      }
      // check if button 2 is pressed
      else if (input_B & 0x02) {
        //pos = (pos < left) ? pos++ : pos;
        pos = high;
        move_motor(freq, pos);
        play = 1;
      }
      break;
    case OFF:
      //turn off tones
      move_servo(0,1500);
      move_motor(0,1000);
      play = 0;
      break;
    default:
      break;
  }

}

int main(void)
{
  DDRA = 0x0F; PORTA = 0xF0;	// set PORTB to inputs/outputs (low/high bits)
  DDRB = 0xF0; PORTB = 0x0F;	// set PORTB to inputs/outputs (low/high bits)
  DDRD = 0xFF; PORTD = 0x00;	// set PORTB to inputs/outputs (low/high bits)

  state = START;              // initialize state
  servo_on();               // enable PWM output
  motor_on();               // enable PWM output

  while(1)
  {
    Tick();                 // step through states
  }
}
