/*
 * File:   main.c
 * Author: windows 7
 *
 * Created on August 13, 2017, 2:08 PM
 */

#include <xc.h>
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
#define _XTAL_FREQ 20000000


#include "Code/LCD_fex.h"
#include"Code/adc_fex.h"

void pwm_init()
{
    PR2 = 0b11111001;
    T2CON = 0b00000100;
    CCP1CON = 0b00111100;
}


void main(void) {
   float adc_value,for_percentage,rev_percentage;
    
    TRISA = 0xff;
    TRISB = 0x00;
    TRISC = 0x00;
    
    PORTB = 0x00;
    PORTC = 0x00;
    
    adc_init();
    InitLCD();
    pwm_init();
    
    while(1)
    {   
        RC0 = ~RC0;     //test
       
        adc_value = read_adc();  
        display_float(adc_value);
        WriteDataToLCD('/');
        
//range-130 to 400 reverse
//range-400 to 600 stop
//range-600 to 700 forward
        
        if(adc_value > 400 && adc_value < 600)
        {
            CCPR1L = 0x00;
            
            WriteStringToLCD("STOP");
            
            RC6 = 0;
            RC7 = 0;
        }
        
        else if(adc_value > 600)               
        {
            for_percentage = ((adc_value - 600)/350);
            
            CCPR1L = (int)(for_percentage * 255);
            
            display_float((for_percentage * 255));
            
            RC6 = 1;                        //forward direction
        }
        
        else if(adc_value < 400)               
        {
            __delay_ms(100);
            if(adc_value < 400)
            {
                rev_percentage = (((adc_value - 400)* -1)/350); //-1 for avoid negative value

                CCPR1L = (int)(rev_percentage * 255);

                display_float(rev_percentage * 255);

                RC7 = 1;                        //reverse direction
            }
        }
        else
        {
            WriteStringToLCD("Error!!");
            __delay_ms(5000);
        }
                    
        //__delay_ms(100);
        ClearLCDScreen();
    }
}
