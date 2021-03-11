/* 
 * Project: D2_I2C_MAESTRO
 * File:    MAESTRO_SPI.c
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: Match, 2021.
 * Source: KHALED MAGDY 
 * Link:  https://deepbluembedded.com/mpu6050-with-microchip-pic-accelerometer-gyroscope-interfacing-with-pic/
 * Link: http://www.electronicwings.com
 */

//============================================================================*/
// LIBRERIAS
//============================================================================*/

#include "USART_Header_File.h"     
#include <xc.h>

//============================================================================*/
// FUNCIONES
//============================================================================*/

void USART_Init(long baud_rate)
{
    TRISC6 = 0;                     // Tx para transmitir datos
    TRISC7 = 1;                     // Rx para recibir
    SPBRG = BAUDRATE;               // SPBRG = (F_CPU /(64*9600))-1 */
    TXSTA = 0x20;                   // Permitir la transmision de datos
    RCSTA = 0x90;                   // Permitir la recepcion de datos
}

void USART_TxChar(char data)
{       
    while(!TXIF);                   // El buffer esta vacio?
    TXREG = data;                   // Se envia dato
}

char USART_RxChar()
{
    if (RCSTAbits.OERR ==0)            // Hay error?
    {
        CREN = 0;                      // Apagar modulo para apagar error
        NOP();
        CREN = 1;                      // Enciende una vez no haya error
    }
    return (RCREG);                    // Se envia valor a RCREG
} 

void USART_SendString(const char *str)
{
   while(*str!='\0')                   // Mientras no este vacio
   {            
        USART_TxChar(*str);            // Se agregan bytes 
        str++;
   }
}

void MSdelay(unsigned int val)
{
     unsigned int i,j;                 // Delay de 1ms para frecuencia de 8MHz
        for(i=0;i<=val;i++)
            for(j=0;j<165;j++);     
}

