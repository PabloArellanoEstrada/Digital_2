/* 
 * Project: Interrupciones y Librerias
 * File:    ADC_lib.h
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 9, 2021,
 */

#ifndef ADC_lib_H
#define	ADC_lib_H

//------------------------------------------------------------------------------
// Funciones de Conversion ADC
//------------------------------------------------------------------------------

#include <xc.h>                // include processor files - each processor file is guarded.  
#include <stdint.h>            // Variables de ancho definido

void initADC (uint8_t CHS);    // Prototipo de funcion

#endif	/* ADC_lib_H */

