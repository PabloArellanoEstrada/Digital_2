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

#ifndef LCD_8bits_H
#define	LCD_8bits_H


#ifndef _XTAL_FREQ
#define	_XTAL_FREQ 8000000
#endif

#ifndef RS
#define	RS PORTCbits.RC0
#endif

#ifndef RW
#define	RW PORTCbits.RC1
#endif

#ifndef E
#define	E  PORTCbits.RC2
#endif

#ifndef D0
#define D0 PORTDbits.RD0
#endif

#ifndef D1
#define D1 PORTDbits.RD1
#endif

#ifndef D2
#define D2 PORTDbits.RD2
#endif

#ifndef D3
#define D3 PORTDbits.RD3
#endif

#ifndef D4
#define D4 PORTDbits.RD4
#endif

#ifndef D5
#define D5 PORTDbits.RD5
#endif

#ifndef D6
#define D6 PORTDbits.RD6
#endif

#ifndef D7
#define D7 PORTDbits.RD7
#endif

//------------------------------------------------------------------------------
// Funciones de Conversion ADC
//------------------------------------------------------------------------------

#include <xc.h>                        // include processor files - each processor file is guarded.  
#include <stdint.h>                    // Variables de ancho definido

void Lcd_Port (char a);
void Lcd_Cmd (char a);

void Lcd_Init();                       // Prototipo de funcion
void Lcd_Clear();
void Lcd_Set_Cursor(char a, char b);
void Lcd_Write_String(char *a);
void Lcd_Shift_Left();
void Lcd_Shift_Right();
void Lcd_Write_Char(char a);
void Lcd_Write_Char_4(char a);

#endif	/* LCD_H */

