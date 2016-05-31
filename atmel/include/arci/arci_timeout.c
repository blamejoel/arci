/*
    AR-CI Timeout Control
*/

#include <arci/config.h>

/* REFERENCE VARS FROM OTHER FILES
    enum TIME_STATES { START_T, INIT_T, NO_DATA_D, DATA_D };
    unsigned char incoming_data;    // tmp value to read incoming data
    unsigned char ready;            // connected to client flag
    unsigned char data_avail        // data received flag
*/

#define ST_OFFLINE  1   // server offline
#define ST_READY    2   // connection enabled
#define ST_DISCON   3   // timeout

int TickFct_T (int state) {
    static unsigned char st_led;        // status LED
    static unsigned short elapsed
    // State Transitions
    switch (state) {
        case START_T:
            state = INIT_T; break;      // transition to INIT_T state
        case INIT_T:
            elapsed = 0;
            ready = 0;
            st_led = ST_OFFLINE;
            state = WAIT_T; break;      // transition to WAIT_T state
        case NO_DATA_D:
            if (data_avail) {
                ready = 1;
                st_led = ST_READY;
                state = DATA_D;
            }
            else {
                elapsed = (elapsed > 65000) ? (MAX_TIMEOUT + 1) : (elapsed + 1);
            }
            break;
        case DATA_D:
            if (!data_avail) {
                elapsed = 0;
                state = NO_DATA_D;
            }
            break;
        default:
            state = START_T;            // error likely happened, restart
            break;
    }
    // State Actions
    switch (state) {
        case NO_DATA_D:
            if (elapsed > MAX_TIMEOUT) {
                ready = 0;
                st_led = ST_DISCON;
            }
            break;
        case DATA_D:
            break;
        default:
            break;
    }
    PORTB = (PORTB & 0xCF) | st_led;
}
