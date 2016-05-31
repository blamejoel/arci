#define CLOCK_MHZ 8000000
#define PRESCALER 64
#define TIMER_CLK 125000
#define TIMER_PERIOD 8

// usefull math -> 1/frequency = period in ms

// takes in frequency of the period and high value of PWM in ms
void move_servo(double frequency, short high) {
    static short current_high;
    if (high != current_high) {
            if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
            else { TCCR3B |= 0x03; } // resumes/continues timer/counter
            // calculate frequency for PWM period
            double pwm_period = (CLOCK_MHZ / (PRESCALER * (frequency - 1)));  //works for 50
            // set TOP value timer will count to (PWM period)
            ICR3 = pwm_period - 1;    // Clock/2*Prescaler*freq

            // calculate us to stay high from ms input
            high = high/TIMER_PERIOD;

            // set count for PWM high
            OCR3A = high;
            //ICR3 = 2549;    // Clock/Prescaler*freq
                            // 8000000/64*49

        current_high = high; // Updates the current frequency
    }
}

// takes in frequency of the period and high value of PWM in ms
void move_motor(double frequency, short high) {
    static short current_high;
    if (high == 0)
        TCCR1B &= 0x08;
    if (high != current_high) {
            if (!frequency) { TCCR1B &= 0x08; } //stops timer/counter
            else { TCCR1B |= 0x03; } // resumes/continues timer/counter
            // calculate frequency for PWM period
            double pwm_period = (CLOCK_MHZ / (PRESCALER * (frequency - 1)));  //works for 50
            // set TOP value timer will count to (PWM period)
            ICR1 = pwm_period - 1;    // Clock/2*Prescaler*freq

            // calculate us to stay high from ms input
            high = high/TIMER_PERIOD;

            // set count for PWM high
            OCR1A = high;
            //ICR3 = 2549;    // Clock/Prescaler*freq
                            // 8000000/64*49

        current_high = high; // Updates the current frequency
    }
}

// takes in frequency of the period and high value of PWM in ms
void move_motor_reverse(double frequency, short high) {
    static short current_high;
    if (high == 0)
        TCCR1B &= 0x08;
    if (high != current_high) {
            if (!frequency) { TCCR1B &= 0x08; } //stops timer/counter
            else { TCCR1B |= 0x03; } // resumes/continues timer/counter
            // calculate frequency for PWM period
            double pwm_period = (CLOCK_MHZ / (PRESCALER * (frequency - 1)));  //works for 50
            // set TOP value timer will count to (PWM period)
            ICR1 = pwm_period - 1;    // Clock/2*Prescaler*freq

            // calculate us to stay high from ms input
            high = high/TIMER_PERIOD;

            // set count for PWM high
            OCR1B = high;
            //ICR3 = 2549;    // Clock/Prescaler*freq
                            // 8000000/64*49

        current_high = high; // Updates the current frequency
    }
}

void servo_on() {
    TCCR3A = (1 << COM3A1) | (1 << COM3B1) | (1 << WGM31);
    //TCCR3A = (1 << COM3A1) | (1 << COM3A0) | (1 << WGM31) | (1 << WGM30);
    //TCCR3A = (1 << COM3A0);   // orig
    // COM3A0: Toggle PB6 on compare match between counter and OCR3A
    TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31) | (1 << CS30);
    //TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30); // orig
    // WGM32: When counter (TCNT3) matches OCR3A, reset counter
    // CS31 & CS30: Set a prescaler of 64
    move_servo(0,0);
}

void motor_on() {
    TCCR1A = (1 << COM3A1) | (1 << COM3B1) | (1 << WGM31);
    //TCCR3A = (1 << COM3A1) | (1 << COM3A0) | (1 << WGM31) | (1 << WGM30);
    //TCCR3A = (1 << COM3A0);   // orig
    // COM3A0: Toggle PB6 on compare match between counter and OCR3A
    TCCR1B = (1 << WGM33) | (1 << WGM32) | (1 << CS31) | (1 << CS30);
    //TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30); // orig
    // WGM32: When counter (TCNT3) matches OCR3A, reset counter
    // CS31 & CS30: Set a prescaler of 64
    move_motor(0,0);
}

void servo_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}
