/* 
 * Project: LCD
 * File:    ADC_lib.h
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 9, 2021,
 */

//============================================================================*/
// LIBRERIA
//============================================================================*/

#ifndef SPI_SPI_H
#define	SPI_SPI_H

#ifndef _XTAL_FREQ
#define	_XTAL_FREQ 4000000
#endif
#include <xc.h>                         
#include <stdint.h>     

//============================================================================*/
// PROTOTIPO DE FUNCIONES 
//============================================================================*/

void SPI_Maestro_Init (char port_mode, char SCK);
void SPI_Esclavo_Init (char port_mode, char SCK);
void SPI_Enviar (char valor);
char SPI_Recibir ();

#endif	/* LCD_H */

