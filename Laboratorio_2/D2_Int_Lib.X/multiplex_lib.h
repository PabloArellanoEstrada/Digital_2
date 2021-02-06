/* 
 * Project: Interrupciones y Librerias
 * File:    multiplex_lib.h
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 9, 2021,
 */

#ifndef multiplex_lib_H
#define	multiplex_lib_H

#include <xc.h>                        // include processor files - each processor file is guarded.  
#include <stdint.h>                    // Variables de ancho definido
 
//------------------------------------------------------------------------------
// Funciones de Multiplexacion
//------------------------------------------------------------------------------

void division (void);                  // Prototipo de Division
void multiplexar (void);               // Prototipo de Multiplexacion
void hexadecimal (uint8_t show);       // Prototipo de Hexadecimal


#endif	/* multiplex_lib_H */

