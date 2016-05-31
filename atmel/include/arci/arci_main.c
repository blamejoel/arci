
#include <avr/io.h>
#include <ucr/scheduler.h>
#include <ucr/timer.h>
#include <arci/arci_config.c>
#include <arci/arci_motor.c>
#include <arci/arci_steering.c>
#include <arci/arci_data.c>
#include <arci/arci_timeout.c>

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    // Task periods
    unsigned long int System_Period_calc = PERIOD;
    unsigned long int Timeout_Task_calc = PERIOD_T;

    // Calculate GCD
    unsigned long int tmpGCD = 1;
    unsigned long int GCD = findGCD(System_Period_calc, Timeout_Task_calc);

    unsigned long int Data_Period, Motor_Period,
            Steering_Period = System_Period_calc/GCD;
    unsigned long int Timeout_Period = Timeout_Task_calc/GCD;

    static task Data_Task, Motor_Task, Steering_Task, Timeout_Task;

    task *tasks[] = { &Data_Task, &Motor_Task, &Steering_Task, &Timeout_Task };

    const unsignedshort numTasks = sizeof(tasks)/sizeof(task*);

    TimerSet(PERIOD);
    TimerOn();
    motor_on();
    servo_on();
    initUSART(0);

    while(1) {
        while(!TimerFlag);
        TimerFlag = 0;
    }
}
