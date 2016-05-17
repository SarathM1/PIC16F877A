
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
#define CS1  RB0

#define WREN 0x06
#define WRITE 0x02

void spi_init()
{
    CS1 = 1;
    TRISC3 = 0;         // SCK=RC3 is the serial clock
    TRISC4 = 1;         // SDI=RC4 is serial data input
    TRISC5 = 0;         // SDO=RC5 is serial data output
    TRISB0 = 0;
    
    SMP = 1;            // Input data sampled at middle data output time
    CKP = 0;            // Idle state for clock is a high level
    CKE = 1;            // Transmit occurs on idle to active clock state
    
    SSPEN = 1;
    SSPIF = 0;
    
    SSPM3 = 0;
    SSPM2 = 0;
    SSPM1 = 0;
    SSPM0 = 0;      // SPI Master mode, clock = FOSC/4
}

unsigned char spi_send(unsigned char data)
{
    SSPBUF = data;
    while(!BF);
    return SSPBUF;
}
void main()
{
    spi_init();
    //Enable the write latch
    CS1 = 0;          //Select device
    spi_send(WREN); //Send 'Enable Write' command
    CS1 = 1;          //Deselect device (required after the above command)
    
    //Select address 0x00
    CS1  = 0;          //Select device
    spi_send(WRITE); //Send 'Write' command

    spi_send(0x00);  //Write the 24-bit address
    spi_send(0x00);  //
    spi_send(0x00);  //

    //Write the data
    spi_send(0b10101010);

    CS1 = 1;          //Deselect device
   
    while(1);
}

