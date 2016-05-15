/*
 WAP to blink an LED every 1 second using timer0 interrupt; Fosc = 4MHz
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

volatile unsigned int count = 0;

void timer_init()
{
    TMR0 = 0;
    TMR0IF = 0;
    GIE = 1;
    TMR0IE = 1;
    T0CS = 0;
    PSA = 0;
}

void interrupt ISR()
{
    if(TMR0IF)
    {
        TMR0IF = 0;
        count++;
    }
}

void main()
{
    TRISB0 = 0;
    RB0    = 0;
    timer_init();
    while(1)
    {
        if(count == 15)
        {
            RB0 = ~RB0;
            count = 0;
        }
    }
}