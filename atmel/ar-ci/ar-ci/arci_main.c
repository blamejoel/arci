#include <avr/io.h>
#include <ucr/usart_ATmega1284.h>
#include <arci/arci-scheduler.h>
#include <arci/arci_config.h>
#include <arci/pwm_servo.c>
#include <arci/arci_data.c>
#include <arci/arci_motor.c>
#include <arci/arci_steering.c>
#include <arci/arci_timeout.c>
#include <arci/arci-timer.h>

unsigned char version = '7';

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    unsigned char SystemPeriodCalc = PERIOD;
    unsigned char TimeoutTaskCalc = PERIOD_T;
    
    unsigned char GCD = PERIOD;

    // Assign periods for scheduler
    unsigned char Data_Period, Motor_Period,
            Steering_Period;
    Data_Period = Motor_Period = Steering_Period = (SystemPeriodCalc/GCD);
    unsigned char Timeout_Period = (TimeoutTaskCalc/GCD);

    // Create tasks and insert into an array of tasks
    static task Data_Task, Motor_Task, Steering_Task, Timeout_Task;
    task *tasks[] = { &Data_Task, &Motor_Task, &Steering_Task, &Timeout_Task };

    // Calculate number of tasks to iterate through
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    // Initialize tasks
    Data_Task.state = START_D;
    Data_Task.period = Data_Period;
    Data_Task.elapsedTime = 0;
    Data_Task.TickFct = &TickFct_D;

    Motor_Task.state = START_M;
    Motor_Task.period = Motor_Period;
    Motor_Task.elapsedTime = 0;
    Motor_Task.TickFct = &TickFct_M;

    Steering_Task.state = START_S;
    Steering_Task.period = Steering_Period;
    Steering_Task.elapsedTime = 0;
    Steering_Task.TickFct = &TickFct_S;

    Timeout_Task.state = START_T;
    Timeout_Task.period = Timeout_Period;
    Timeout_Task.elapsedTime = 0;
    Timeout_Task.TickFct = &TickFct_T;

    TimerSet(PERIOD);
    TimerOn();

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

    unsigned char i;
    while(1) {
      for(i = 0; i < numTasks; i++) {
          if(tasks[i]->elapsedTime == tasks[i]->period) {
              tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
              tasks[i]->elapsedTime = 0;
          }
          tasks[i]->elapsedTime += 1;
      }
      while(!TimerFlag);
      TimerFlag = 0;
    }
}
