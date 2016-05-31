/*
    AR-CI Motor Control
*/

#include <arci/pwm_servo.c>
#include <arci/config.h>

/* REFERENCE VARS FROM OTHER FILES
    enum MOTOR_STATES { START_M, INIT_M, WAIT_M, MOVE_M };
    unsigned char incoming_data;    // tmp value to read incoming data
    unsigned char ready;            // connected to client flag
*/

int TickFct_M (int state) {
    static unsigned short motor_high;      // pwm high time
    static unsigned char speed;            // speed input nibble
    static unsigned char rev;              // reverse flag
    // State Transitions
    switch (state) {
        case START_M:
            state = INIT_M; break;
        case INIT_M:
            speed = 0;
            move_motor(0,0);
            state = WAIT_M; break;
        case WAIT_M:
            if (!ready) {
                move_motor(0,0);
            }
            else {
                rev = (incoming_data & 0x80) ? 1 : 0;
                speed = (incoming_data & 0x0F);
                state = MOVE_M;
            }
            break;
        case MOVE_M:
            state = WAIT_M; break;
        default:
            state = START_M;              // error likely happened, restart
            break;
    }
    // State Actions
    switch (state) {
        case MOVE_M:
            switch(speed) {
                case 0:
                    motor_high = 0;
                    break;
                case 1:
                    motor_high = SLOW_SPEED;
                    break;
                case 2:
                    motor_high = (SLOW_SPEED + STEP_SPEED);
                    break;
                case 3:
                    motor_high = (SLOW_SPEED + (STEP_SPEED * (speed - 1)));
                    break;
                case 4:
                    motor_high = (SLOW_SPEED + (STEP_SPEED * (speed - 1)));
                    break;
                case 5:
                    motor_high = (SLOW_SPEED + (STEP_SPEED * (speed - 1)));
                    break;
                case 6:
                    motor_high = (SLOW_SPEED + (STEP_SPEED * (speed - 1)));
                    break;
                case 7:
                    motor_high = (SLOW_SPEED + (STEP_SPEED * (speed - 1)));
                    break;
                case 8:
                    motor_high = (SLOW_SPEED + (STEP_SPEED * (speed - 1)));
                    break;
                case 9:
                    motor_high = (SLOW_SPEED + (STEP_SPEED * (speed - 1)));
                    break;
                case 10:
                    motor_high = (SLOW_SPEED + (STEP_SPEED * (speed - 1)));
                    break;
                case 11:
                    motor_high = (SLOW_SPEED + (STEP_SPEED * (speed - 1)));
                    break;
                case 12:
                    motor_high = (SLOW_SPEED + (STEP_SPEED * (speed - 1)));
                    break;
                case 13:
                    motor_high = (SLOW_SPEED + (STEP_SPEED * (speed - 1)));
                    break;
                case 14:
                    motor_high = (SLOW_SPEED + (STEP_SPEED * (speed - 1)));
                    break;
                case 15:
                    motor_high = MAX_SPEED;
                    break;
                default:
                    motor_high = 0;
                    break;
            }
            if (rev) {
                // stop fwd direction
                move_motor(0,0);

                // limit reverse speed
                motor_high = (bottom > 3) ? MAX_REV : motor_high;
                move_motor_reverse(FREQ,motor_high);
            }
            else {
                // stop reverse direction
                move_motor_reverse(0,0);
                move_motor(FREQ,motor_high);
            }
            break;
        default:
            break;
    }
}
