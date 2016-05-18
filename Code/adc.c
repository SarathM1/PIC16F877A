

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

#define _XTAL_FREQ 20000000
#include "LCD.h"

void adc_init()
{
    TRISA = 0xff;         // AN0
    ADCON1 = 0x00;
    ADCON0 = 0x81;
}

int read_adc()
{
    int value = 0;
    GO_nDONE = 1;
    while(GO_nDONE);
    value = ((ADRESH<<2) + (ADRESL>>6));
    return value;
}

void display(int value)
{   
    int i;
    char r;
    char arr[11];
    for(i=0; value; i++,value/=10)
    {
        r = value%10;
        arr[i] = r + 0x30;
    }
    arr[i] = '\0';
    
    for(;i>=0;i--)
        WriteDataToLCD(arr[i]);
}
void main()
{
    int value;
    
    adc_init();
    InitLCD();
    TRISB = 0x00;
    WriteStringToLCD("H3110 w0r1d");
    while(1)
    {
        value = read_adc();
        display(value);
        __delay_ms(100);
        ClearLCDScreen();
    }
}
