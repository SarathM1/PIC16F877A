/*
 WAP to blink an LED every 0.5 second using timer1 interrupt; Fosc = 4MHz
 */
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

volatile int count = 0;

void timer_init()
{
    GIE  = 1;
    PEIE = 1;
    TMR1IE = 1;
    TMR1IF = 0;
    
    TMR1H = 0;
    TMR1L = 0;
    T1CKPS0 = 1;  // Since Prescalar = 1:2    
}

void interrupt ISR()
{
    if(TMR1IF == 1)
    {
        TMR1IF = 0;
        count++;
    }
}

void main()
{
    timer_init();
    TRISB0 = 0;
    RB0 = 0;
    TMR1ON = 1;
    while(1)
    {
        if(count == 4)
        {
            RB0 = ~ RB0;
            count = 0;
        }
    }
}