/*
 * File            : I2C.h
 * Author          : Ligo George
 * Company         : electroSome
 * Project         : I2C Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * Link: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Modificada por: Pablo Mazariegos con la ayuda del auxiliar Gustavo Ordoñez 
 * Basado en Link: http://microcontroladores-mrelberni.com/i2c-pic-comunicacion-serial/
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __I2C_LIB_H
#define	__I2C_LIB_H

#include <xc.h>                         /* Include processor files - each processor file is guarded. */
#define F_OSC 8000000                   /* Define F_OSC, here it's 8 MHz */
#define I2C_CLOCK 100000                /* I2C clock frequency is 100 kHz*/
#define BITRATE ((F_OSC/(4*I2C_CLOCK))-1) /* find bit rate to assign this value to SSPADD register*/

void I2C_Ready();                       /* Check weather I2C is ready/idle or not */
void I2C_Init();                        /* Initialize I2C configuration*/
char I2C_Start(char);                   /* Send START pulse with slave device write address */
void I2C_Start_Wait(char);              /* Send START pulse with slave device write address until acknowledgement */
char I2C_Repeated_Start(char);          /* Send REPEATED START pulse with slave device read address*/
char I2C_Stop();                        /* Send STOP pulse*/
char I2C_Write(unsigned char);          /* Write data/address to slave device */
void I2C_Ack();                         /* Send acknowledge to slave for continue read */
void I2C_Nack();                        /* Send negative acknowledge to slave for stop current communication */
char I2C_Read(char);                    /* Read data from slave devices with 0=Ack & 1=Nack */
void MSdelay(unsigned int val);     /* millisecond delay function */


#endif	/* __I2C_H */