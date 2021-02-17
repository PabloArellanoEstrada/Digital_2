/* 
 * Project: LCD
 * File:    ADC_lib.h
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

#ifndef LCD_SPI_H
#define	LCD_SPI_H

#define	RS PORTEbits.RE0
#define	RW PORTEbits.RE2
#define	E  PORTEbits.RE1

#define D0 PORTDbits.RD0
#define D1 PORTDbits.RD1
#define D2 PORTDbits.RD2
#define D3 PORTDbits.RD3
#define D4 PORTDbits.RD4
#define D5 PORTDbits.RD5
#define D6 PORTDbits.RD6
#define D7 PORTDbits.RD7

#ifndef _XTAL_FREQ
#define	_XTAL_FREQ 8000000
#endif
#include <xc.h>                         
#include <stdint.h>     

//============================================================================*/
// PROTOTIPO DE FUNCIONES 
//============================================================================*/

void Lcd_Port (char a);
void Lcd_Cmd (char a);
void Lcd_Init(void);                       
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Write_String(char *a);
void Lcd_Shift_Left(void);
void Lcd_Shift_Right(void);
void Lcd_Write_Char(char a);
void Lcd_Write_Char_4(char a);

#endif	/* LCD_H */

