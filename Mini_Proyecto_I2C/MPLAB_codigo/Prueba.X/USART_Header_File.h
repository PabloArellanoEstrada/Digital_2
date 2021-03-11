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

#ifndef USART_HEADER_FILE_H
#define	USART_HEADER_FILE_H

#include <pic16f887.h>             /* Include PIC18F4550 header file */
#define F_CPU 8000000/64            /* Define ferquency */
#define BAUDRATE (((float)(F_CPU)/(float)baud_rate)-1)/* Define Baud value */

//============================================================================*/
// FUNCIONES
//============================================================================*/

void USART_Init(long);              /* USART Initialization function */
void USART_TxChar(char);            /* USART character transmit function */
char USART_RxChar();                /* USART character receive function */
void USART_SendString(const char *);/* USART String transmit function */
void MSdelay(unsigned int val);     /* millisecond delay function */
void botones();


#endif	/* USART_HEADER_FILE_H */

