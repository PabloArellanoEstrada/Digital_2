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

void  USART_lib_config()                 // Valor del pic a compu de dos potensiometros
{
    SPBRG = 25;  // velocidad transmision
     
    TXSTAbits.CSRC = 0;
    TXSTAbits.TX9  = 0;
    TXSTAbits.TXEN = 1;
    TXSTAbits.SYNC = 0;
    TXSTAbits.SENDB= 0;
    TXSTAbits.BRGH = 1;
    TXSTAbits.TRMT = 0;
    TXSTAbits.TX9D = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9  = 0;
    RCSTAbits.SREN = 0;
    RCSTAbits.CREN = 1;
    RCSTAbits.ADDEN= 0;
    RCSTAbits.FERR = 0;
    RCSTAbits.OERR = 0;
    RCSTAbits.RX9D = 0;
    
    PIR1bits.RCIF = 0;              // limpiar bandera
    PIE1bits.RCIE = 1;              // habilitar bandera por interrupcion de datos
    PIR1bits.TXIF = 0;
}