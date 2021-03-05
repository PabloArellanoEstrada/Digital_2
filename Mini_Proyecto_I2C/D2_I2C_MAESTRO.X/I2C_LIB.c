/*
 * File            : I2C.c
 * Author          : Ligo George
 * Company         : electroSome
 * Project         : I2C Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * Link: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Modificada por: Pablo Mazariegos con la ayuda del auxiliar Gustavo Ordoñez 
 * Basado en Link: http://microcontroladores-mrelberni.com/i2c-pic-comunicacion-serial/
 */
#include "I2C_LIB.h"
//*****************************************************************************
// Función para inicializar I2C Maestro
//*****************************************************************************
void I2C_Master_Init(const unsigned long c)
{
    SSPCON2 = 0;                      // no se modifica nada 
    SSPADD = (_XTAL_FREQ/(4*c))-1;    // frecuencia de reloj
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;             // entradas
    TRISCbits.TRISC4 = 1;             // entradas
    SSPCON = 0b00101000;              // no collision, no overflow, enable, hold clock low, i2c master mode
}
//*****************************************************************************
// Función de espera: mientras se esté iniciada una comunicación,
// esté habilitado una recepción, esté habilitado una parada
// esté habilitado un reinicio de la comunicación, esté iniciada
// una comunicación o se este transmitiendo, el IC2 PIC se esperará
// antes de realizar algún trabajo
//*****************************************************************************
void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); // si hay transmision en progreso, o si haya inicio aknoldewge, hay condicion de parada
                                                  // o esperando a que no este ocupado
}
//*****************************************************************************
// Función de inicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Start()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.SEN = 1;    //inicia la comunicación i2c
}
//*****************************************************************************
// Función de reinicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RSEN = 1;   //reinicia la comunicación i2c
}
//*****************************************************************************
// Función de parada de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Stop()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.PEN = 1;    //detener la comunicación i2c
}
//*****************************************************************************
//Función de transmisión de datos del maestro al esclavo
//esta función devolverá un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
void I2C_Master_Write(unsigned d)
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPBUF = d;             
}
//*****************************************************************************
//Función de recepción de datos enviados por el esclavo al maestro
//esta función es para leer los datos que están en el esclavo
//*****************************************************************************
unsigned short I2C_Master_Read(unsigned short a) 
{
    unsigned short temp;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas recepcion de datos
    temp = SSPBUF;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas valor en variable temporal
    if(a == 1){                     
        SSPCON2bits.ACKDT = 0;      // acknowledge
    }else{                          
        SSPCON2bits.ACKDT = 1;      // not acknowledge
    }
    SSPCON2bits.ACKEN = 1;          // Iniciar sequencia de Acknowledge
    return temp;                    // Regresar valor del dato leído
}
//*****************************************************************************
// Función para inicializar I2C Esclavo
//*****************************************************************************
void I2C_Slave_Init(uint8_t address)
{ 
    SSPADD = address;
    SSPCON = 0x36;      // 0b00110110   no collision, no overflow, enaable, release clock, i2c slave 7 bit address
    SSPSTAT = 0x80;     // 0b10000000   slew rate disabled
    SSPCON2 = 0x01;     // 0b00000001   clow swithcing is enable
    TRISC3 = 1;         // entrada
    TRISC4 = 1;         // entrada
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}
//*****************************************************************************

void I2C_Start_Wait(char slave_write_address)
{
  while(1)
  {   
    SSPCON2bits.SEN = 1;            /* Send START condition */
    while(SSPCON2bits.SEN);         /* Wait for completion of START */
    SSPIF = 0;
    if(!SSPSTATbits.S)              /* Continue if START is failed */
        continue;
    I2C_Master_Write(slave_write_address); /* Write slave device address with write to communicate */
    if(ACKSTAT)                     /* Check whether Acknowledgment received or not */
    {
        I2C_Master_Stop();                 /* If not then initiate STOP and continue */
        continue;
    }    
    break;                          /* If yes then break loop */
  }
}

void MSdelay(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<=val;i++)
            for(j=0;j<165;j++);      /*This count Provide delay of 1 ms for 8MHz Frequency */
}

char I2C_Repeated_Start(char slave_read_address)
{
    RSEN = 1;                       /* Send REPEATED START condition */
    while(SSPCON2bits.RSEN);        /* Wait for completion of REPEATED START condition */
    SSPIF = 0;
    if(!SSPSTATbits.S)              /* Return 0 if REPEATED START is failed */
    return 0;
    I2C_Write(slave_read_address);  /* Write slave device address with read to communicate */
    if (ACKSTAT)                    /* Return 2 if acknowledgment received else 1 */
     return 1;
    else
     return 2;
}