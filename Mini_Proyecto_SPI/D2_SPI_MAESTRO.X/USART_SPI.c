
/* 
 * Project: LCD
 * File:    USART.h
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 9, 2021,
 */
//============================================================================*/
// LIBRERIA
//============================================================================*/

#include "USART_SPI.h"  

//============================================================================*/
// FUNCIONES
//============================================================================*/

void  USART_lib_config()                 
{
    SPBRG = 25;  
     
    TXSTAbits.CSRC = 0;     // No importa, solo para sincrono
    TXSTAbits.TX9  = 0;     // 8 bits
    TXSTAbits.TXEN = 1;     // Permitir transmision
    TXSTAbits.SYNC = 0;     // Asincrono
    TXSTAbits.SENDB= 0;     // 
    TXSTAbits.BRGH = 1;     // Alta velocidad por 4Mhz   
    TXSTAbits.TRMT = 0;     //
    TXSTAbits.TX9D = 0;     // 
    
    RCSTAbits.SPEN = 1;     // Permitir puerto serial
    RCSTAbits.RX9  = 0;     // 8 bits
    RCSTAbits.SREN = 0;     // No importa solo para asincrono
    RCSTAbits.CREN = 1;     // Permitir recibir
    RCSTAbits.ADDEN= 0;     // 
    RCSTAbits.FERR = 0;     // 
    RCSTAbits.OERR = 0;     // 
    RCSTAbits.RX9D = 0;     // 
    
    PIR1bits.RCIF = 0;      // Bandera recibir apagada   
    PIE1bits.RCIE = 1;      // 
    PIR1bits.TXIF = 0;      // Bandera transmitir apagada
}