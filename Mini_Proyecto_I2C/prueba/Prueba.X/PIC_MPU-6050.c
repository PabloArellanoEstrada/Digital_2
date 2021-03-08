/* 
 * Project: D2_I2C_MAESTRO
 * File:    MAESTRO_SPI.c
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 18, 2021.
 */

//============================================================================*/
// LIBRERIAS
//============================================================================*/
 
#include <pic16f887.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "USART_Header_File.h"      /* Include USART header file */
#include "I2C_Master_File.h"
#include "MPU6050_res_define.h"

//============================================================================*/
// PALABRA DE CONFIGURACION
//============================================================================*/

#include "Configuration_header_file.h"

//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void MPU6050_Init();
void MPU_Start_Loc();


//============================================================================*/
// CICLO PRINCIPAL
//============================================================================*/

void main()

{
	int Ax,Ay,Az,T,Gx,Gy,Gz;
    OSCCON = 0x72;
    I2C_Init();											/* Initialize I2C */	
	MPU6050_Init();										/* Initialize Gyro */
	USART_Init(9600);									/* Initialize USART with 9600 baud rate */
  
	while(1)
	{
		MPU_Start_Loc();								/* Read Gyro values continuously and send to terminal over USART */
		Ax = (((int)I2C_Read(0)<<8) | (int)I2C_Read(0));
		Ay = (((int)I2C_Read(0)<<8) | (int)I2C_Read(0));
		Az = (((int)I2C_Read(0)<<8) | (int)I2C_Read(0));
		T =  (((int)I2C_Read(0)<<8) | (int)I2C_Read(0));
		Gx = (((int)I2C_Read(0)<<8) | (int)I2C_Read(0));
		Gy = (((int)I2C_Read(0)<<8) | (int)I2C_Read(0));
		Gz = (((int)I2C_Read(0)<<8) | (int)I2C_Read(1));
		I2C_Stop();
        
        char Ax1 = Ax / 256;
        char Ay1 = Ay / 256;
        char Az1 = Az / 256;
        
        Ax1 = Ax1 * 4;
        Ay1 = Ay1 * 4;
        Az1 = Az1 * 4;
             
		///////////////////////////////////////////////////////////////////////  Ax
        
        char decimal = Ax1 /1000;
        char residuo0 = Ax1 % 1000;
        char entero = residuo0 / 100;
        char residuo1 = residuo0 % 100;
        char unidad = residuo1 / 10;
        char residuo2 = residuo1 % 10;
        char decena = residuo2 / 1;
         
        USART_TxChar(65);
        USART_TxChar(120);
        USART_TxChar(61);
        USART_TxChar(32);
        USART_TxChar(decimal+48);
        USART_TxChar(entero+48);
        USART_TxChar(46);
        USART_TxChar(unidad+48);
        USART_TxChar(decena+48);
        USART_TxChar(32);
        USART_TxChar(32);
        
         /////////////////////////////////////////////////////////////////////// Ay
        
        decimal = Ay1 /1000;
        residuo0 = Ay1 % 1000;
        entero = residuo0 / 100;
        residuo1 = residuo0 % 100;
        unidad = residuo1 / 10;
        residuo2 = residuo1 % 10;
        decena = residuo2 / 1;
         
        USART_TxChar(65);
        USART_TxChar(121);
        USART_TxChar(61);
        USART_TxChar(32);
        USART_TxChar(decimal+48);
        USART_TxChar(entero+48);
        USART_TxChar(46);
        USART_TxChar(unidad+48);
        USART_TxChar(decena+48);
        USART_TxChar(32);
        USART_TxChar(32);
        
        ///////////////////////////////////////////////////////////////////////  Az
        
		decimal = Az1 /1000;
        residuo0 = Az1 % 1000;
        entero = residuo0 / 100;
        residuo1 = residuo0 % 100;
        unidad = residuo1 / 10;
        residuo2 = residuo1 % 10;
        decena = residuo2 / 1;
         
        USART_TxChar(65);
        USART_TxChar(122);
        USART_TxChar(61);
        USART_TxChar(32);
        USART_TxChar(decimal+48);
        USART_TxChar(entero+48);
        USART_TxChar(46);
        USART_TxChar(unidad+48);
        USART_TxChar(decena+48);
        USART_TxChar(32);
        USART_TxChar(32);
        USART_TxChar('\n');   
        
        //TRISA = 0;                                          // Puerto A como salida 
                
        //uint8_t leer = USART_RxChar(); 
        
        //PORTAbits.RA1 = 1;
        //PORTAbits.RA2 = 1;
       
	}
}

//============================================================================*/
// FUNCIONES
//============================================================================*/


void MPU6050_Init()										/* Gyro initialization function */
{
	MSdelay(150);		/* Power up time >100ms */
	I2C_Start_Wait(0xD0);	/* Start with device write address */
	I2C_Write(SMPLRT_DIV);	/* Write to sample rate register */
	I2C_Write(0x07);	/* 1KHz sample rate */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(PWR_MGMT_1);	/* Write to power management register */
	I2C_Write(0x01);	/* X axis gyroscope reference frequency */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(CONFIG);	/* Write to Configuration register */
	I2C_Write(0x00);	/* Fs = 8KHz */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(GYRO_CONFIG);	/* Write to Gyro configuration register */
	I2C_Write(0x18);	/* Full scale range +/- 2000 degree/C */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(INT_ENABLE);	/* Write to interrupt enable register */
	I2C_Write(0x01);
	I2C_Stop();

}

void MPU_Start_Loc()
{
  I2C_Start_Wait(0xD0);    //
  I2C_Write(ACCEL_XOUT_H); //
  I2C_Repeated_Start(0xD1);
  I2C_Read(0); // dummy
}


