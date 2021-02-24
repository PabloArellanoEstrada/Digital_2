
/* 
 * Project: D2_SPI_MAESTRO
 * File:    ADC_SPI.c
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 9, 2021,
 * Libreria https://electrosome.com/lcd-pic-mplab-xc8/
 * Autor:   Ligo George
 * Autor 2: Pablo Mazariegos (Canvas)
 */

//============================================================================*/
// LIBRERIA
//============================================================================*/

#include "LCD_SPI.h"       // Libreria LCD

//============================================================================*/
// FUNCIONES
//============================================================================*/

void Lcd_Port (char a)
{
    PORTD = a;             // Escribir a todo puerto D
}

void Lcd_Cmd (char a)
{
    RS = 0;                // Comando hacia LCD
    Lcd_Port(a);           // Mover dato a puerto
    E = 1;                 // E de 1 a 0 para indicar transmision
    __delay_ms(5);
    E = 0;
}

void Lcd_Init()
{
    Lcd_Port(0b0000000);    // A puerto
    __delay_ms(20);
    Lcd_Cmd(0b00110000);    // Modo 8 bits  
    RS = 0;
    RW = 0;
    __delay_ms(5);
    Lcd_Cmd(0b00110000);    // Modo 8 bits
     RS = 0;
    RW = 0;
    __delay_ms(11);
    Lcd_Cmd(0b00110000);    // Modo 8 bits
    RS = 0;
    RW = 0;
    Lcd_Cmd(0b00111000);    // Dos lineas a 5x11 caracteres
    Lcd_Cmd(0b00001100);    // Activado y encendido
    Lcd_Cmd(0b00000001);    // Visualizador borrado
    Lcd_Cmd(0b00000110);    // Por incremento y desplazamiento desactivado
}

void Lcd_Clear()
{
    Lcd_Cmd(0b00000001);    // Limpiar
}

void Lcd_Set_Cursor(char a, char b)
{
    char temp;
    if (a == 1)             // Fila 1?
    {
        temp  = 0x80 + b;   
        Lcd_Cmd(temp);      // Escribir a partir de 00hex
    }
    else if (a == 2)        // Fila 2?
    {
        temp  = 0xC0 + b;
        Lcd_Cmd(temp);      // Escribir a partir de 40hex
    }
}
    
void Lcd_Write_String(char *a)
{
    int i;
    for (i = 0; a[i] != '\0'; i++)  // Hasta que este vacio?
        Lcd_Write_Char(a[i]);       // Escribir cadena caracteres
}

void Lcd_Shift_Left()
{
    Lcd_Cmd(0x18);          // Correr a la izquierda 0001_1000
}

void Lcd_Shift_Right()
{
    Lcd_Cmd(0x1C);          // Correr a la derecha 0001_1100        
}

void Lcd_Write_Char(char a) 
{
    RS = 1;                 // Puerto como datos
    Lcd_Port(a);            // Puerto a
    E = 1;                  // De 1 a 0 mientras que se transmiten datos
    __delay_us(40);
    E = 0;
}


