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

#include "USART.h"  


void  USART_lib_config()                      // Valor del pic a compu de dos potensiometros
{
    TXSTAbits.TX9 = 0;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 0;
    TXSTAbits.TXEN = 1;

    SPBRG = .12;
    SPBRGH = 0;

    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    RCSTAbits.SPEN = 1;

    PIR1bits.RCIF = 0;
    PIR1bits.TXIF = 0;
}