/*	arci_usart.c - 5/18/2016 9:45:15 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #  Exercise #
 *	Exercise Description:
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#include <ucr/usart_ATmega1284.h>
#include <ucr/pwm_servo.c>
#include <ucr/arci-timer.h>

#define PERIOD 20
#define FREQ 50.0
#define STEP_LEFT 30
#define STEP_RIGHT 30
#define MIDDLE 1450

// @ 50Hz
//#define SLOW_SPEED 6000
//#define MAX_SPEED 20250
//#define STEP_SPEED 101

// @ 250Hz
#define MAX_SPEED 4040
#define STEP_SPEED 171
#define SLOW_SPEED 1640
#define MAX_REV (SLOW_SPEED + (STEP_SPEED*2))

unsigned char version = '0';
enum States { START, INIT, WAIT } state;      // state vars
unsigned char tmp, ready;               // tmp value to read incoming data
unsigned short steer, speed;            // steering/speed values to servo/motor
unsigned char top, bottom;              // steering/speed input from user
unsigned char rev;                      // reverse flag

void Tick() {
    static unsigned char last;          // steering var
    // State Transitions
    switch (state) {
        case START:
            state = INIT; break;        // transition to INIT state
        case INIT:
            ready = tmp = rev = 0;
            top = 0x40;
            bottom = 0x00;
            move_motor(0,0);
            move_servo(0,0);
            state = WAIT; break;        // transition to WAIT state
        case WAIT:
            if (!ready) {
                move_motor(FREQ,0);
                ready = 1;
            }
            else
            if (USART_HasReceived(0)) {
                tmp = USART_Receive(0);

                rev = (tmp & 0x80) ? 1 : 0;
                top = (tmp & 0x70) >> 4;
                bottom = (tmp & 0x0F);

                PORTC = bottom | (top << 4);
                /*if (USART_IsSendReady(0)) {
                    USART_Send(tmp,0);
                }*/
            }
            break;
        default:
            state = START;              // error likely happened, restart
            break;
    }

    // State Actions
    switch (state) {
        case WAIT:
            switch(top) {
                case 0:
                    steer = (MIDDLE + (STEP_RIGHT*3));
                    break;
                case 1:
                    steer = (MIDDLE + (STEP_RIGHT*2));
                    break;
                case 2:
                    steer = (MIDDLE + STEP_RIGHT);
                    break;
                case 3:
                    steer = MIDDLE;
                    break;
                case 4:
                    steer = MIDDLE;
                    break;
                case 5:
                    steer = (MIDDLE - (STEP_LEFT));
                    break;
                case 6:
                    steer = (MIDDLE - (STEP_LEFT*2));
                    break;
                case 7:
                    steer = (MIDDLE - (STEP_LEFT*3));
                    break;
                default:
                    steer = MIDDLE;
                    break;
            }
            if (last != top) {
                last = top;
                move_servo(FREQ,steer);
            }
            else {
                move_servo(0,0);
            }
            switch(bottom) {
                case 0:
                    speed = 0;
                    break;
                case 1:
                    speed = SLOW_SPEED;
                    break;
                case 2:
                    speed = (SLOW_SPEED + STEP_SPEED);
                    break;
                case 3:
                    speed = (SLOW_SPEED + (STEP_SPEED*2));
                    break;
                case 4:
                    speed = (SLOW_SPEED + (STEP_SPEED*3));
                    break;
                case 5:
                    speed = (SLOW_SPEED + (STEP_SPEED*4));
                    break;
                case 6:
                    speed = (SLOW_SPEED + (STEP_SPEED*5));
                    break;
                case 7:
                    speed = (SLOW_SPEED + (STEP_SPEED*6));
                    break;
                case 8:
                    speed = (SLOW_SPEED + (STEP_SPEED*7));
                    break;
                case 9:
                    speed = (SLOW_SPEED + (STEP_SPEED*8));
                    break;
                case 10:
                    speed = (SLOW_SPEED + (STEP_SPEED*9));
                    break;
                case 11:
                    speed = (SLOW_SPEED + (STEP_SPEED*10));
                    break;
                case 12:
                    speed = (SLOW_SPEED + (STEP_SPEED*11));
                    break;
                case 13:
                    speed = (SLOW_SPEED + (STEP_SPEED*12));
                    break;
                case 14:
                    speed = (SLOW_SPEED + (STEP_SPEED*13));
                    break;
                case 15:
                    speed = MAX_SPEED;
                    break;
                default:
                    speed = 0;
                    break;
            }
            if (rev) {
                move_motor(FREQ,0);
                speed = (bottom > 3) ? MAX_REV : speed;
                move_motor_reverse(FREQ,speed);
            }
            else {
                move_motor_reverse(FREQ,0);
                move_motor(FREQ,speed);
            }
            break;
        default:
            break;
    }
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// set PORTA to inputs and initialize
	DDRB = 0xFF; PORTB = 0x00;	// set PORTB to outputs and initialize
	DDRC = 0xFF; PORTC = 0x00;	// set PORTC to outputs and initialize
	DDRD = 0xFF; PORTD = 0x00;	// set PORTD to outputs and initialize

    state = START;              // initialize state
    TimerSet(PERIOD);
    TimerOn();

    motor_on();
    servo_on();
    initUSART(0);
    USART_Send('r',0);
    USART_Send('e',0);
    USART_Send('a',0);
    USART_Send('d',0);
    USART_Send('y',0);
    USART_Send(' ',0);
    USART_Send('v',0);
    USART_Send(version,0);
    USART_Send('\n',0);

	while(1)
	{
		Tick();                 // step through states
        while (!TimerFlag);     // Wait for a period
        TimerFlag = 0;          // reset the timer flag
	}
}
