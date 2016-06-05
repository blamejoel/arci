/*
    AR-CI Data Control
*/

#include "arci_config.h"
#include <ucr/usart_ATmega1284.h>

/* REFERENCE VARS FROM OTHER FILES
    enum DATA_STATES { START_D, INIT_D, WAIT_D, READ_D };
    unsigned char incoming_data;    // tmp value to read incoming data
*/

int TickFct_D(int state) {
    // State Transitions
    switch(state) {
        case START_D:
            state = INIT_D; break;        // transition to INIT_D state
        case INIT_D:
            incoming_data = data_avail = 0;
            state = WAIT_D; break;        // transition to WAIT_D state
        case WAIT_D:
            if(USART_HasReceived(0)) {
                data_avail = 1;
                // if(!wired)
                    state = READ_D;
                /*if (USART_IsSendReady(0)) {
                    USART_Send(incoming_data,0);
                }*/
            }
            else {
                data_avail = 0;
            }
            break;
        case READ_D:
            state = WAIT_D; break;
        default:
            state = START_D;              // error likely happened, restart
            break;
    }
    // State Actions
    switch(state) {
        case READ_D:
            incoming_data = USART_Receive(0);
            break;
        default:
            break;
    }
    return state;
}
