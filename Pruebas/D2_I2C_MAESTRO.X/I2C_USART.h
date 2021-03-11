/* 
 * Project: D2_SPI_MAESTRO
 * File:    USART_SPI.c
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 18, 2021.
 */

//============================================================================*/
// LIBRERIA
//============================================================================*/

#ifndef I2C_USART_H
#define	I2C_USART_H

#include <pic16f887.h>             /* Include PIC18F4550 header file */
#define F_CPU 8000000/64            /* Define ferquency */
#define BAUDRATE (((float)(F_CPU)/(float)baud_rate)-1)/* Define Baud value */

#include <xc.h>                  // XC8 libreria     
#include <stdint.h>              // Variables de ancho definido   
#include <stdio.h>               // Tipos de variables, macros, entradas y salidas

//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/
void USART_Init(long);              /* USART Initialization function */
void USART_lib_config(void);         

#endif	/* I2C_USART_H */

