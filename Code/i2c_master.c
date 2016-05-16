#include <xc.h>
#include "LCD.h"

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

#define _XTAL_FREQ 8000000
#define SCL_DIR TRISC4
#define SDA_DIR TRISC3
#define SDA RC4
#define SCL RC3

void initI2c()
{
    SCL_DIR = SDA_DIR = 1;
    SDA = SCL = 0;
    SSPSTAT = 0x80; // Disable slew rate
    SSPCON = 0x28; // SSPEN = 1; i2c master mode
}

void i2cStart()
{
    SEN = 1;
    while(!SSPIF);
    SSPIF = 0;
}

void i2cWrite(unsigned char addr)
{
    SSPBUF = addr;
    while(!SSPIF);
    SSPIF = 0;
}

void i2cStop()
{
    PEN = 1;
    while(!SSPIF);
    SSPIF = 0;
}

void write_time(unsigned char sec, unsigned char min, unsigned char hr)
{
    i2cStart();
    i2cWrite(0xD0);
    i2cWrite(0x00);
    i2cWrite(sec);
    i2cWrite(min);
    i2cWrite(hr);
    i2cStop();
}

void write_date(unsigned char week, unsigned char date, unsigned char month,unsigned char year)
{
    i2cStart();
    i2cWrite(0xD0);
    i2cWrite(0x03);
    i2cWrite(week);
    i2cWrite(date);
    i2cWrite(month);
    i2cWrite(year);
    i2cStop();
}

void i2cRepStart()
{
    RSEN = 1;
    while(!SSPIF);
    SSPIF = 0;
}

int i2cRead()
{
    RCEN = 1;
    while(!SSPIF);
    SSPIF = 0;
    return SSPBUF;
}

void i2cAck()
{
    ACKDT = 0;
    ACKEN = 1;
    while(!SSPIF);
    SSPIF = 0;
}

void i2cNoAck()
{
    ACKDT = 1;
    ACKEN = 1;
    while(!SSPIF);
    SSPIF = 0;
}

void read_time(unsigned char *sec,unsigned char *min,unsigned char *hr, unsigned char *pm)
{
    i2cStart();
    i2cWrite(0xD0);
    i2cWrite(0x00);
    i2cRepStart();
    i2cWrite(0xD1);
    *sec = i2cRead();
    i2cAck();
    *min = i2cRead();
    i2cAck();
    *hr  = i2cRead();
    i2cNoAck();
    i2cStop();
    
    if(*hr & (1<<5))
        *pm = 1;
    else
        *pm = 0;
}

void display_time(unsigned char sec, unsigned char min, unsigned char hr, unsigned char pm)
{
    WriteDataToLCD( ((hr&0x0F)>>4) + 0x30);
        WriteDataToLCD( (hr & 0x0F) + 0x30);
        
        WriteDataToLCD(':');
        
        WriteDataToLCD( (min>>4) + 0x30);
        WriteDataToLCD( (min&0x0F) + 0x30);
        
        WriteDataToLCD(':');
        
        WriteDataToLCD( (sec >> 4) + 0x30);
        WriteDataToLCD( (sec & 0x0F) + 0x30);
        
        if(pm)
            WriteStringToLCD(" pm");
        else
            WriteStringToLCD(" am");
}

void main()
{
    unsigned char sec,min,hr,pm;
    TRISA = 0;
    PORTA = 0;
    InitLCD();
    ClearLCDScreen();
    initI2c();
    write_time(0x07,0x32,0x48);
    write_date(0x06,0x15,0x12,0x12);
    
    while(1)
    {
        read_time(&sec, &min, &hr, &pm);
        display_time(sec,min,hr,pm);
        
        __delay_ms(1000);
        RA0 = !RA0;
        ClearLCDScreen();
    }
}

