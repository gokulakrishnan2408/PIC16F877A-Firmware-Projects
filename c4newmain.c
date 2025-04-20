/*
 * File:   c4newmain.c
 * Author: gokul
 *
 * Created on 24 March, 2025, 12:28 AM
 */
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 6000000 //6MHZ 
void init(void);
void keyscan(void);
void batterystatus(float);
void lcd_cd(unsigned char);
void lcd_dt(unsigned char);
void lcd_op(float);
unsigned char array[15]={"BATT VOLT:    V"};
unsigned char bv1[11]={"BATT LOW   "},bv2[11]={"BATT MEDIUM"},bv3[11]={"BATT HIGH  "};
unsigned char value,point;
unsigned int x,d1,d2,d3;
float j=17.5;

void main(void) {
    init();
    point = '.';
    while(1)
    {
        keyscan();
    }
}

void init(void)
{
    TRISC = 0x00;  //PORTC AS OUTPUT
    TRISD = 0x00;  //PORTD AS OUTPUT
    TRISB = 0xF0;  // 1111 0000 RB4 TO RB7 AS INPUT
    lcd_cd(0x30);        //initialize
    __delay_ms(100);
    lcd_cd(0x30);        
    __delay_ms(100);
    lcd_cd(0x30);
    __delay_ms(100);
    lcd_cd(0x38);       //N and F initialize
    __delay_ms(100);
    lcd_cd(0x0C);       //cursor off
    __delay_ms(100);
    lcd_cd(0x01);       //clear display
    __delay_ms(100);
}
void lcd_cd(unsigned char i)
{
    PORTC &=~0X08; //RS = 0  AT RC3
    PORTD = i;
    PORTC |=0X01;  //EN = 1 AT RC0
    __delay_ms(10);
    PORTC &=~0X01; //EN = 0 AT RC0
}
void lcd_dt(unsigned char i)
{
  PORTC |=0X08; //RS = 1  AT RC3
    PORTD = i;
    PORTC |=0X01;  //EN = 1 AT RC0
    __delay_ms(10);
    PORTC &=~0X01; //EN = 0 AT RC0
}  

void lcd_op(float i)    //to display int value
{
    unsigned int m =(unsigned int)(i*10);
    d3 =m/100;
    d2 =(m - (d3*100))/10;
    d1 = m - (d3*100)-(d2 * 10);
    lcd_dt(0x30 + (unsigned char)d3);   //ASIIC CONVERT
    lcd_dt(0x30 + (unsigned char)d2);
    lcd_dt(point);
    lcd_dt(0x30 + (unsigned char)d1);
}

void keyscan()            //FUNCTION TO SCAN THE KEYS
{
    value = PORTB & 0xF0;   //AND WITH MASK VALUE GET INPUT FROM RB4 TO RB7
    switch(value)
    {
        case 0xE0:             //1110 0000 case 1
            lcd_cd(0X80);      //SET POSITION 80 ROW 1 
            for(x=0;x<10;x++)         
            {
                lcd_dt(array[x]);
            }
            lcd_cd(0x8E);
            lcd_dt(array[14]);
            lcd_cd(0x8A);
            lcd_op(j);
            batterystatus(j);     
            break;
        case 0xD0:          //1101 0000 case 2
            j+=0.1;
            if(j>22.5)
            {
                j=22.5;
            }
            lcd_cd(0x8A);
            lcd_op(j);
            batterystatus(j);
            break;
        case 0xB0:          //1011 0000 case 3
            j-=0.1;
            if(j<15.5)
            {
                j=15.5;   
            }
            lcd_cd(0x8A);
            lcd_op(j);
            batterystatus(j);
            break;
        case 0x70:          //1011 0000 case 4
            j=17.5;
            lcd_cd(0x8A);
            lcd_op(j);
            batterystatus(j);
            break;
    }   
}
void batterystatus(float j)       //FUNCTION OT SHOW BATTERY STATUS
{
    lcd_cd(0xC0);                 // SET POSITION AS C0 SECOND ROW
    if(j>=15.5 & j<=17.5)         //DISPLAY BATT LOW
    {
        for(x=0;x<11;x++)         
        {
            lcd_dt(bv1[x]);       
        }
    }
    if(j>=17.6 & j<20.6)          //DISPLAY BATT MEDIUM
    { 
        for(x=0;x<11;x++)         
        {
            lcd_dt(bv2[x]);
        }
    }
    if(j>=20.6 & j<=22.5)         //DISPLAY BATT HIGH
    {
        for(x=0;x<11;x++)
        {
            lcd_dt(bv3[x]);
        }
    }
}

