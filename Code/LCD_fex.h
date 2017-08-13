
// Function Declarations for Generic Functions.c

#ifndef __LCD
#define __LCD

// Define Pins
#define LCD_E    			RB1     // Enable pin for LCD
#define LCD_RS	 			RB0     // RS pin for LCD
#define LCD_Data_Bus_D4		RB4		// Data bus bit 4
#define LCD_Data_Bus_D5		RB5		// Data bus bit 5
#define LCD_Data_Bus_D6		RB6		// Data bus bit 6
#define LCD_Data_Bus_D7		RB7		// Data bus bit 7

// Define Pins direction registrers
#define LCD_E_Dir     			TRISB1
#define LCD_RS_Dir   	 		TRISB0
#define LCD_Data_Bus_Dir_D4   	TRISB4
#define LCD_Data_Bus_Dir_D5     TRISB5
#define LCD_Data_Bus_Dir_D6  	TRISB6
#define LCD_Data_Bus_Dir_D7 	TRISB7

// Constants
#define E_Delay       20  


// Function Declarations
void WriteCommandToLCD(unsigned char);
void WriteDataToLCD(char);
void InitLCD(void);
void WriteStringToLCD(const char*);
void ClearLCDScreen(void);
void display_float(float val);

#endif