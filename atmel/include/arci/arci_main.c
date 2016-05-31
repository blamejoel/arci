
#include <avr/io.h>
#include <avr/sleep.h>
#include <arci/arci-timer.h>
#include <arci/arci_config.h>
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

    // Recalculate GCD periods for scheduler
    unsigned long int Data_Period, Motor_Period,
            Steering_Period = System_Period_calc/GCD;
    unsigned long int Timeout_Period = Timeout_Task_calc/GCD;

    // Create tasks and insert into an array of tasks
    static task Data_Task, Motor_Task, Steering_Task, Timeout_Task;
    task *tasks[] = { &Data_Task, &Motor_Task, &Steering_Task, &Timeout_Task };

    // Calculate number of tasks to iternate through
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    // Initialize tasks
    Data_Task.state = START_D;
    Data_Task.period = Data_Period;
    Data_Task.elapsedTime = Data_Period;
    Data_Task.TickFct = &TickFct_D;

    Motor_Task.state = START_M;
    Motor_Task.period = Motor_Period;
    Motor_Task.elapsedTime = Motor_Period;
    Motor_Task.TickFct = &TickFct_M;

    Steering_Task.state = START_S;
    Steering_Task.period = Steering_Period;
    Steering_Task.elapsedTime = Steering_Period;
    Steering_Task.TickFct = &TickFct_S;

    Timeout_Task.state = START_T;
    Timeout_Task.period = Timeout_Period;
    Timeout_Task.elapsedTime = Timeout_Period;
    Timeout_Task.TickFct = &TickFct_T;

    motor_on();
    servo_on();
    initUSART(0);

    TimerSet(GCD);
    TimerOn();

    set_sleep_mode(SLEEP_MODE_PWR_SAVE);

    while(1) {
      sleep_enable();
      sleep_mode();
      sleep_disable();
    }
}
