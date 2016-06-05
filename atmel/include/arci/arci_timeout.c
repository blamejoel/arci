/*
    AR-CI Timeout Control
*/

//#include <arci/arci_config.h>

/* REFERENCE VARS FROM OTHER FILES
    enum TIME_STATES { START_T, INIT_T, NO_DATA_D, DATA_D };
    unsigned char incoming_data;    // tmp value to read incoming data
    unsigned char ready;            // connected to client flag
    unsigned char data_avail        // data received flag
*/

#define ST_OFFLINE  0x01   // server offline
#define ST_READY    0x02   // connection enabled
#define ST_DISCON   0x03   // timeout

int TickFct_T(int state) {
    static unsigned char st_led;        // status LED
    static unsigned char elapsed;
    static unsigned char ready_cnt;
    // State Transitions
    switch (state) {
        case START_T:
            ready_cnt = 0;
            state = INIT_T; break;      // transition to INIT_T state
        case INIT_T:
            elapsed = 0;
            ready = 0;
            st_led = ST_OFFLINE;
            if(ready_cnt >= 9) {
                state = NO_DATA_D;
            }
            else if(data_avail) {
                ready_cnt += 1;
            }
            break;      // transition to WAIT_T state
        case NO_DATA_D:
            if(crash_alert == 1 && (incoming_data & 0x80)) {
                ready = 1;
                state = ATTN_D;
            }
            else if(crash_alert == 2 && !(incoming_data & 0x80)) {
                ready = 1;
                state = ATTN_D;
            }
            else if(~PINA & 0x03) {
                ready = 0;
                state = ATTN_D;
            }
            else if(!(~PINA & 0x03) && data_avail) {
                ready = 1;
                st_led = ST_READY;
                state = DATA_D;
            }
            else {
                elapsed = (elapsed > 250) ? (MAX_TIMEOUT + 1) : (elapsed + 1);
            }
            break;
        case DATA_D:
            if(!data_avail) {
                elapsed = 0;
                state = NO_DATA_D;
            }
            break;
        case ATTN_D:
            state = NO_DATA_D;
            break;
        default:
            state = START_T;            // error likely happened, restart
            break;
    }
    // State Actions
    switch(state) {
        case NO_DATA_D:
            if(elapsed > MAX_TIMEOUT) {
                ready = 0;
                if(!crash_alert) {
                    st_led = ST_DISCON;
                }
            }
            break;
        case ATTN_D:
            crash_alert = (~PINA & 0x03);
            elapsed = (elapsed > 250) ? (MAX_TIMEOUT + 1) : (elapsed + 1);
            if(!(elapsed % (PERIOD))) {
                st_led = (st_led & ST_OFFLINE) ? ST_READY : ST_OFFLINE;
            }
            break;
        default:
            break;
    }
    PORTB = (PORTB & 0xFC) | st_led;
    return state;
}
