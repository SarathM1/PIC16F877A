/*
 Serial communication in PIC controller
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

#define STRLEN 5
volatile unsigned char t;
volatile unsigned char rcIndx = 0;
volatile unsigned char rcArray[STRLEN];
void uart_init()
{
    TXEN = 1;
    CREN = 1;
    BRGH = 1;
    
    SPBRG = 129;
    
    GIE = 1;
    PEIE = 1;
    RCIE = 1;
    RCIF = 0;
    
    TRISC7 = 1;
    TRISC6 = 1;
    SPEN = 1;
}

void uart_txChar(unsigned char ch)
{
    while(!TRMT);
    TXREG = ch;
}

void uart_txStr(unsigned const char *str)
{
    while(*str != '\0')
    {
        uart_txChar(*str);
        str++;
    }
}

void interrupt ISR()
{
    if(RCIF)
    {
        t = RCREG;
        // uart_txChar(t);
        
        if(t=='\n')
        {
            RC5 = !RC5;
        }
        if( (t != '\n') && (rcIndx <STRLEN))
        {
            rcArray[rcIndx++] = t;
            // rcArray[rcIndx + 1] = '\0';
            // RC5 = !RC5;
        }
        else
        {
            rcArray[rcIndx+1] = '\0';
            uart_txStr((const char*)rcArray);
            rcIndx = 0;
        }
        RCIF = 0;
    }
}

void main()
{
    TRISC5 = 0;
    RC5 = 0;
    uart_init();
    uart_txStr("USART ready\n");
    while(1);
}