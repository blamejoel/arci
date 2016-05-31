/*
    AR-CI Steering Control
*/

#include <arci/pwm_servo.c>
#include <arci/config.h>

/* REFERENCE VARS FROM OTHER FILES
    enum SERVO_STATES { START_S, INIT_S, WAIT_S, UPDATE_S };
    unsigned char incoming_data;    // tmp value to read incoming data
    unsigned char ready;            // connected to client flag
*/

int TickFct_S(int state) {
    static unsigned short servo_high;   // pwm high time
    static unsigned char cur_angle;         // steering angle nibble
    static unsigned char last;          // steering var
    // State Transitions
    switch(state) {
        case START_S:
            state = INIT_S; break;      // transition to INIT_S state
        case INIT_S:
            cur_angle = 0x30;               // 'zero' out steering
            move_servo(0,0);
            state = WAIT_S; break;        // transition to WAIT_S state
        case WAIT_S:
            if(!ready) {
                move_servo(0,0);
            }
            else {
                cur_angle = (incoming_data & 0x70) >> 4;
                state = UPDATE_S;
            }
            break;
        case UPDATE_S:
            state = WAIT_S; break;
        default:
            state = START_S;              // error likely happened, restart
            break;
    }
    // State Actions
    switch(state) {
        case UPDATE_S:
            switch(cur_angle) {
                case 0:
                    servo_high = (MIDDLE + (STEP_RIGHT*3));
                    break;
                case 1:
                    servo_high = (MIDDLE + (STEP_RIGHT*2));
                    break;
                case 2:
                    servo_high = (MIDDLE + STEP_RIGHT);
                    break;
                case 3:
                    servo_high = MIDDLE;
                    break;
                case 4:
                    servo_high = (MIDDLE - (STEP_LEFT));
                    break;
                case 5:
                    servo_high = (MIDDLE - (STEP_LEFT*2));
                    break;
                case 6:
                    servo_high = (MIDDLE - (STEP_LEFT*3));
                    break;
                default:
                    servo_high = MIDDLE;
                    break;
            }

            // following block allows servo to 'relax' after receiving direction
            // new angle is different from previous angle
            if(last != cur_angle) {
                last = cur_angle;
                move_servo(FREQ,servo_high);
            }
            // same angle, go limp
            else {
                move_servo(0,0);
            }
            break;
        default:
            break;
    }
}
