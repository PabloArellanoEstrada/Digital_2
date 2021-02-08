/* 
 * Project: Interrupciones y Librerias
 * File:    ADC_lib.h
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 9, 2021,
 * Libreria https://electrosome.com/lcd-pic-mplab-xc8/
 * Autor:   Ligo George
 * Autor 2: Pablo Mazariegos (Canvas)
 */

#include "LCD_8bits.h"  

void Lcd_Port (char a)
{
    PORTD = a;
}

void Lcd_Cmd (char a)
{
    RS = 0;                // Comando hacia LCD
    Lcd_Port(a);
    E = 1;
    __delay_ms(4);
    E = 0;
}

void Lcd_Init()
{
    Lcd_Port(0b0000000);     // puerto
    __delay_ms(20);
    Lcd_Cmd(0b00110000);      
    RS = 0;
    RW = 0;
    __delay_ms(5);
    Lcd_Cmd(0b00110000);
     RS = 0;
    RW = 0;
    __delay_ms(11);
    Lcd_Cmd(0b00110000);
    RS = 0;
    RW = 0;
    
    Lcd_Cmd(0b00111000);
    Lcd_Cmd(0b00001000);
    Lcd_Cmd(0b00000001);
    Lcd_Cmd(0b00000110);

}

void Lcd_Clear()
{
    Lcd_Cmd(0);
    Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b)
{
    char temp, z, y;
    if (a == 1)
    {
        temp  = 0b00010000 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    }
    else if (a == 2)
    {
        temp  = 0b11000000 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    }
}

void Lcd_Write_String(char *a)
{
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Left()
{
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}

void Lcd_Shift_Right()
{
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}

void Lcd_Write_Char(char a) 
{
    RS = 1;
    Lcd_Port(a);      
    E = 1;
    __delay_us(40);
    E = 0;
}


