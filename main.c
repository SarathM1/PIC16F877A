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

#define FRWD_RELAY RC6
#define REVERSE_RELAY RC7
#define ON 1
#define OFF 0


#include "Code/LCD_fex.h"
#include"Code/adc_fex.h"

void pwm_init()
{
    PR2 = 0b11111001;
    T2CON = 0b00000100;
    CCP1CON = 0b00111100;
}

void printAdcPeriod(int adc_val, float pwm_val)
{
    display_float(adc_val);
    WriteDataToLCD('/');
    
    if(pwm_val==-1)
        WriteStringToLCD(" STOP");
    else
        display_float(pwm_val);
    WriteStringToLCD("   ");
}

void stop_motor()
{
    CCPR1L = 0x00;
    FRWD_RELAY = OFF;
    REVERSE_RELAY = OFF;
}

void move_forward()
{
    REVERSE_RELAY = OFF;                        //reverse direction
    __delay_ms(5);
    FRWD_RELAY = ON;                        //forward direction
}

void move_reverse()
{
    FRWD_RELAY = OFF;                        //forward direction
    __delay_ms(5);
    REVERSE_RELAY = ON;                        //reverse direction
}
void main(void) {
   float adc_value,for_percentage,rev_percentage;
   float pwm_val;
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
        
        
//range-130 to 400 reverse
//range-400 to 600 stop
//range-600 to 700 forward
        
        if(adc_value > 400 && adc_value < 600)
        {
            __delay_ms(80);
            
            printAdcPeriod(adc_value,-1);
             
            stop_motor();
        }
        
        else if(adc_value >= 600)               
        {
            __delay_ms(80);
            adc_value = read_adc();
            if(adc_value >= 600){
                for_percentage = ((adc_value - 600)/350);
                pwm_val = (int)(for_percentage * 240);
                CCPR1L = pwm_val;
                printAdcPeriod(adc_value, pwm_val);
                
                move_forward();
            }
        }
        
        else if(adc_value <= 400)               
        {
            __delay_ms(80);
            adc_value = read_adc();
            if(adc_value <= 400)
            {
                rev_percentage = (((adc_value - 400)* -1)/350); //-1 for avoid negative value
                pwm_val = (int)(rev_percentage * 240);
                CCPR1L = pwm_val;
                printAdcPeriod(adc_value, pwm_val);
                
                move_reverse();
            }
        }
        else
        {
            WriteStringToLCD("Error/ STOP");
            __delay_ms(5000);
            
            stop_motor();
        }
                    
        WriteCommandToLCD(0x80);  // Cursor to beginning of line 1
    }
}
