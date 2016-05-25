

// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>
#include <stdlib.h>

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

void display_float(float val);
void adc_init()
{
    TRISA = 0xff;         // AN0
    ADCON1 = 0x00;
    ADCON0 = 0x81;
}

int read_adc()
{
    GO_nDONE = 1;
    while(GO_nDONE);
    return ((ADRESH<<2) + (ADRESL>>6));
}

float convertToVolt(int adc_value)
{
    float res;
    res = (float)adc_value;
    res = (res * 5/1023);
    return res;
}

void display_float(float val)
{
    char * buf;
    int status;
    buf = ftoa(val, &status);
    buf[5] = '\0';                  //Float precision Adjustment
    WriteStringToLCD(buf);
}

void main()
{
    int adc_value;
    float an0;
    adc_init();
    InitLCD();
    TRISB = 0x00;
    WriteStringToLCD("H3110 w0r1d");
    while(1)
    {
        adc_value = read_adc();
        an0 = convertToVolt(adc_value);
        display_float(an0);
        __delay_ms(10);
        ClearLCDScreen();
    }
}
