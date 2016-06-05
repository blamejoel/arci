/*
    AR-CI Wired Control
*/

#include "arci_config.h"

#define WIRED_CTRL 2

unsigned char cnt = 0;

/* REFERENCE VARS FROM OTHER FILES
    enum WIRED_STATES { START_W, INIT_W, WAIT_W, CMD_W };
    unsigned char wired;            // connected to wired remote flag
*/

void ADC_init();
void Set_A2D_Pin(unsigned char pinNum);

int TickFct_W(int state) {
    static unsigned short a_val;
    // static unsigned char top, tmpC;
    // State Transitions
    switch(state) {
        case START_W:
            state = INIT_W; break;        // transition to INIT_W state
        case INIT_W:
            ADC_init();
            Set_A2D_Pin(WIRED_CTRL);
            wired = 0;
            state = WAIT_W; break;        // transition to WAIT_W state
        case WAIT_W:
            break;
        case CMD_W:
            state = WAIT_W; break;
        default:
            state = START_W;              // error likely happened, restart
            break;
    }
    // State Actions
    switch(state) {
        case WAIT_W:
            a_val = ADC;
            // top = (char)(a_val >> 2) & 0xC0;
            // tmpC = (char)a_val;
            break;
        default:
            break;
    }
    // if(cnt >= 10) {
    //     PORTA = (top & 0xF0) | top;
    //     PORTC = tmpC;
    //     cnt = 0;
    // }
    // else {
    //     cnt++;
    // }
    if(a_val > 522 && a_val < 543)      // up
        wired = 0x3A;
        // tmpC = 0x10;
    else if(a_val > 165 && a_val < 180)   // left
        wired = 0x60;
        // tmpC = 0x01;
    else if(a_val > 146 && a_val < 164)   // up/left
        wired = 0x6A;
        // tmpC = 0x11;
    else if(a_val > 112 && a_val < 120)   // right
        wired = 0x00;
        // tmpC = 0x02;
    else if(a_val > 75 && a_val < 112)   // up/right
        wired = 0x0A;
        // tmpC = 0x12;
    else if(a_val > 61 && a_val < 70)   // down
        wired = 0xBA;
        // tmpC = 0x20;
    else if(a_val >= 56 && a_val <= 61)   // down/left
        wired = 0xEA;
        // tmpC = 0x21;
    else if(a_val > 50 && a_val < 56)   // down/right
        wired = 0x8A;
        // tmpC = 0x22;
    else
        wired = 0x30;
    // PORTC = tmpC;
    return state;
}

void ADC_init() {
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
    // ADEN: setting this bit enables analog-to-digital conversion.
    // ADSC: setting this bit starts the first conversion.
    // ADATE: setting this bit enables auto-triggering. Since we are
    // in Free Running Mode, a new conversion will trigger whenever
    // the previous conversion completes.
}

void Set_A2D_Pin(unsigned char pinNum) {
    ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
    // Allow channel to stabilize
    static unsigned char i = 0;
    for(i = 0; i < 15; i++) { asm("nop"); }
}
