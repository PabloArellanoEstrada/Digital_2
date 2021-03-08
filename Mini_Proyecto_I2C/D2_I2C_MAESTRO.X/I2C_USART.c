/* 
 * Project: D2_SPI_MAESTRO
 * File:    USART_SPI.c
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 9, 2021,
 */
//============================================================================*/
// LIBRERIA
//============================================================================*/

#include "I2C_USART.h"      // Libreria USART

//============================================================================*/
// FUNCIONES
//============================================================================*/

void  USART_lib_config()                 
{
  
     
    TXSTAbits.CSRC = 0;     // No importa, solo para sincrono
    TXSTAbits.TX9  = 0;     // 8 bits
    TXSTAbits.TXEN = 1;     // Permitir transmision
    TXSTAbits.SYNC = 0;     // Asincrono
    TXSTAbits.SENDB= 0;     // 
    TXSTAbits.BRGH = 0;     // Alta velocidad por 8Mhz   
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

void USART_Init(long baud_rate)
{
    TRISC6 = 0;                     /* Make Tx pin as output */
    TRISC7 = 1;                     /* Make Rx pin as input */
    SPBRG = BAUDRATE;               /* Baud rate, SPBRG = (F_CPU /(64*9600))-1 */
    TXSTA = 0x20;                   /* Transmit Enable(TX) enable */ 
    RCSTA = 0x90;                   /* Receive Enable(RX) enable and serial port enable */
}
