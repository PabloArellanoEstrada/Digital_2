
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

#include <pic16f887.h>
#include "I2C_Master_File.h"
#include "USART_Header_File.h"  

//============================================================================*/
// FUNCIONES
//============================================================================*/

void I2C_Init()                     // Configuracion
{
    TRISC3 = 1;                     // Bits como entradas
	TRISC4 = 1;
	SSPSTAT = 80;                  
    SSPCON = 0x28;					// Maestro con Clock = FOSC / (4 * (SSPADD+1))
	SSPCON2 = 0;                    // No recibir, no AKN, no comunicacion, no se modifica nada
    SSPADD = BITRATE;               // frecuencia a (xtal/(4*c))-1 
    SSPIE = 1;                      // Permite las interrupciones
    SSPIF = 0;                      // Estan apagadas
}

char I2C_Start(char slave_write_address)  // Inicio I2C
{   
    SSPCON2bits.SEN = 1;            // Envia condicion inicial
    while(SSPCON2bits.SEN);         // Mientras se de la condicino inicial
    SSPIF=0;                        // Apaga interrupcion
    if(!SSPSTATbits.S)              // Prueba si comunicacion inicial es exitosa
    return 0;
    return (I2C_Write(slave_write_address)); // Indica la direccion del esclavo para escribirle
}

void I2C_Start_Wait(char slave_write_address)  // Si hay transmision en progreso 
{
  while(1)                          
  {   
    SSPCON2bits.SEN = 1;            // Condicion inicial
    while(SSPCON2bits.SEN);         // Mientras espera condicion inicial
    SSPIF = 0;                      // Se apaga la bandera
    if(!SSPSTATbits.S)              // Si el bit inicial no fue detectado continua
        continue;
    I2C_Write(slave_write_address); // Indica la direccion del esclavo para escribirle
    if(ACKSTAT)                     // Hay inicio de ACK? 
    {
        I2C_Stop();                 // Hay condicion de parada?
        continue;
    }    
    break;                          // Si ya no esta ocupado, despues ya puede seguir 
  }                                 // Si no se queda alli esperando
} 

char I2C_Repeated_Start(char slave_read_address)  // Poner el RCEN = 1 para reiniciar comunicacion
{
    RSEN = 1;                       // Condicion de repetir el inicio
    while(SSPCON2bits.RSEN);        // Espera a que se de la condicion inicial
    SSPIF = 0;                      // Se apaga la bandera
    if(!SSPSTATbits.S)              // Si el bit inicial no fue detectado regresa
    return 0;
    I2C_Write(slave_read_address);  // Indica la direccion del esclavo para escribirle
    if (ACKSTAT)                    // Si se recibe el AKN?
     return 1;
    else
     return 2;
}

char I2C_Write(unsigned char data)  // Para escribir
{
      SSPBUF = data;                // Se escribe dato al bufer
      I2C_Ready();
      if (ACKSTAT)                  // Si se recibe el AKN?
        return 1;
      else
        return 2;
}

void I2C_Ack()                      // Si enviamos AKN -  AKN = 0, NAKN = 1
{
    ACKDT = 0;  					
	ACKEN = 1;        				// Habilitamos para enviar
    while(ACKEN);
}

void I2C_Nack()                     // Si enviamos AKN -  AKN = 0, NAKN = 1
{
    ACKDT = 1;          			
	ACKEN = 1;              		// Habilitamos para enviar	          
    while(ACKEN);
}
char I2C_Read(char flag)            // Leer dato del esclavo AKN = 0, NAKN = 1
{
    char buffer;
    RCEN = 1;                       // Habilita recepcion
    while(!SSPSTATbits.BF);         // Espera a que el buffer este lleno
    buffer = SSPBUF;                // Leer valor
    if(flag==0)
        I2C_Ack();                  // Enviamos AKN
    else
        I2C_Nack();                 // No se envia AKN
    I2C_Ready();
    return(buffer);
}

char I2C_Stop()                     // Colocar PEN = 1
{
    PEN = 1;                        // Se inicializa condicion de parada
    while(PEN);                     // Mientras este la condicion de parada
    SSPIF = 0;                      // Bandera de interrupcion se apaga
    if(!SSPSTATbits.P);             // Por si falla el Stop, se regresa un 0
    return 0;
}

void I2C_Ready()
{
    while(!SSPIF);                  // Mientras se apaga bandera de interrupcion
    SSPIF=0;                        // Bandera de interrupcion se apaga
}







