/* 
 * Project: D2_SPI_MAESTRO
 * File:    SPI_SPI.c
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 18, 2021,
 */
//============================================================================*/
// LIBRERIA
//============================================================================*/

#include <xc.h>                        // XC8 libreria
#include "SPI_SPI.h"                   // Libreria SPI    

//============================================================================*/
// FUNCIONES
//============================================================================*/

void SPI_Maestro_Init (char port_mode, char SCK)
{
    TRISCbits.TRISC0 = 0;              // SS Pot
    TRISCbits.TRISC1 = 0;              // SS Botones
    TRISCbits.TRISC2 = 0;              // SS Semaforo
    TRISCbits.TRISC5 = 0;              // SDO out_maestro / in_esclavo
    TRISCbits.TRISC4 = 1;              // SDI in_maestro / out_esclavo
    TRISCbits.TRISC3 = 0;              // CLK
    
    switch (port_mode)
    {
        case 0:
            SSPCONbits.SSPM3  = 0;     // SPI Master Mode Fosc/4
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 0;
            
            break;
         case 1:
            SSPCONbits.SSPM3  = 0;     // SPI Master Mode Fosc/16
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 1;
            
            break;
         case 2:
            SSPCONbits.SSPM3  = 0;     // SPI Master Mode Fosc/64
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 1;
            SSPCONbits.SSPM0  = 0;
            
            break;
         case 3:
            SSPCONbits.SSPM3  = 0;     // SPI Master Mode TMR2
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 1;
            SSPCONbits.SSPM0  = 1;
            break;  
         case 4:
            SSPCONbits.SSPM3  = 0;     // Slave Mode SS enable
            SSPCONbits.SSPM2  = 1;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 0;
            break;
         case 5:
            SSPCONbits.SSPM3  = 0;     // Slave Mode SS disable 
            SSPCONbits.SSPM2  = 1;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 1;
            break;   
    }
    
    switch (SCK)
    {
        case 0:
            SSPCONbits.CKP  = 0;       // CLK Low Level
            SSPSTATbits.CKE = 0;       // Transmision en borde de subida
            
            break;
         case 1:
            SSPCONbits.CKP  = 1;       // CLK High Level
            SSPSTATbits.CKE = 0;       // Transmision en borde de caida
            
            break;
         case 2:
            SSPCONbits.CKP  = 0;       // CLK Low Level
            SSPSTATbits.CKE = 1;       // Transmision en borde de caida
            
            break;
         case 3:
            SSPCONbits.CKP  = 1;       // CLK High Level
            SSPSTATbits.CKE = 1;       // Transmision en borde de subida
            break;    
    }
    SSPSTATbits.SMP = 0;               // Dato muestrado en la mitad del dato de salida
    SSPCONbits.SSPEN = 1;              // Habilita puertos para comunicacion serial
}


void SPI_Esclavo_Init (char port_mode, char SCK)
{
    TRISAbits.TRISA5 = 1;              // SS para comunicar con maestro
    TRISCbits.TRISC5 = 0;              // SDO in_maestro / out_esclavo
    TRISCbits.TRISC4 = 1;              // SDI out_maestro / in_esclavo 
    TRISCbits.TRISC3 = 1;              // CLK
    
    switch (port_mode)
    {
        case 0:
            SSPCONbits.SSPM3  = 0;     // SPI Master Mode Fosc/4
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 0;
            
            break;
         case 1:
            SSPCONbits.SSPM3  = 0;     // SPI Master Mode Fosc/16
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 1;
            
            break;
         case 2:
            SSPCONbits.SSPM3  = 0;     // SPI Master Mode Fosc/64
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 1;
            SSPCONbits.SSPM0  = 0;
            
            break;
         case 3:
            SSPCONbits.SSPM3  = 0;     // SPI Master Mode TMR2
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 1;
            SSPCONbits.SSPM0  = 1;
            break;  
         case 4:
            SSPCONbits.SSPM3  = 0;     // Slave Mode SS enable
            SSPCONbits.SSPM2  = 1;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 0;
            
            break;
         case 5:
            SSPCONbits.SSPM3  = 0;     // Slave Mode SS disable 
            SSPCONbits.SSPM2  = 1;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 1;
            break;   
    }
    
    switch (SCK)
    {
        case 0:
            SSPCONbits.CKP  = 0;       // CLK Low Level
            SSPSTATbits.CKE = 0;       // Transmision en borde de subida
            
            break;
         case 1:
            SSPCONbits.CKP  = 1;       // CLK High Level
            SSPSTATbits.CKE = 0;       // Transmision en borde de caida
            
            break;
         case 2:
            SSPCONbits.CKP  = 0;       // CLK Low Level
            SSPSTATbits.CKE = 1;       // Transmision en borde de caida
            
            break;
         case 3:
            SSPCONbits.CKP  = 1;       // CLK High Level
            SSPSTATbits.CKE = 1;       // Transmision en borde de subida
            break;    
    }
    SSPSTATbits.SMP = 0;               // Dato muestrado en la mitad del dato de salida
    SSPCONbits.SSPEN = 1;              // Habilita puertos para comunicacion serial
}

void SPI_Enviar (char valor)
{
    SSPBUF = valor;                    // Registro que retiene dato de SSPSR hasta que el dato este listo
}


char SPI_Recibir ()
{
    return SSPBUF;                     // Envia byte de buffer que recibe de SSPSR (Registro de Transmision/Recepcion)
}

