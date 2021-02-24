/* 
 * Project: D2_SLAVE_POT
 * File:    SPI_SPI.h
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 18, 2021.
 */

//============================================================================*/
// LIBRERIA
//============================================================================*/

#ifndef SPI_SPI_H
#define	SPI_SPI_H

#ifndef _XTAL_FREQ
#define	_XTAL_FREQ 8000000       // Frecuencia compilador
#endif
#include <xc.h>                  // XC8 libreria              
#include <stdint.h>              // Variables de ancho definido

//============================================================================*/
// PROTOTIPO DE FUNCIONES 
//============================================================================*/

void SPI_Maestro_Init (char port_mode, char SCK);
void SPI_Esclavo_Init (char port_mode, char SCK);
void SPI_Enviar (char valor);
char SPI_Recibir ();

#endif	/* SPI_SPI_H */

