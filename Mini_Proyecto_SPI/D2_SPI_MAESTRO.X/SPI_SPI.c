#include <xc.h>
#include "SPI_SPI.h"

void SPI_Maestro_Init (char port_mode, char SCK)
{
    TRISCbits.TRISC0 = 0;  // ss
    TRISCbits.TRISC1 = 0;  // ss
    TRISCbits.TRISC2 = 0;  // ss
    TRISCbits.TRISC5 = 0;  // SDO out_maestro / in_esclavo
    TRISCbits.TRISC4 = 1;  // SDI in_maestro / out_esclavo
    TRISCbits.TRISC3 = 0;  // clk
    
    switch (port_mode)
    {
        case 0:
            SSPCONbits.SSPM3  = 0;
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 0;
            
            break;
         case 1:
            SSPCONbits.SSPM3  = 0;
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 1;
            
            break;
         case 2:
            SSPCONbits.SSPM3  = 0;
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 1;
            SSPCONbits.SSPM0  = 0;
            
            break;
         case 3:
            SSPCONbits.SSPM3  = 0;
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 1;
            SSPCONbits.SSPM0  = 1;
            break;  
         case 4:
            SSPCONbits.SSPM3  = 0;
            SSPCONbits.SSPM2  = 1;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 0;
            
            break;
         case 5:
            SSPCONbits.SSPM3  = 0;
            SSPCONbits.SSPM2  = 1;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 1;
            break;   
    }
    
    switch (SCK)
    {
        case 0:
            SSPCONbits.CKP  = 0;
            SSPSTATbits.CKE = 0;
            
            break;
         case 1:
            SSPCONbits.CKP  = 1;
            SSPSTATbits.CKE = 0;
            
            break;
         case 2:
            SSPCONbits.CKP  = 0;
            SSPSTATbits.CKE = 1;
            
            break;
         case 3:
            SSPCONbits.CKP  = 1;
            SSPSTATbits.CKE = 1;
            break;    
    }
    SSPSTATbits.SMP = 0;
    SSPCONbits.SSPEN = 1;
}


void SPI_Esclavo_Init (char port_mode, char SCK)
{
    TRISAbits.TRISA5 = 1;  // ss
    TRISCbits.TRISC5 = 0;  // SDO in_maestro / out_esclavo
    TRISCbits.TRISC4 = 1;  // SDI out_maestro / in_esclavo 
    TRISCbits.TRISC3 = 1;  // clk
    
    switch (port_mode)
    {
        case 0:
            SSPCONbits.SSPM3  = 0;
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 0;
            
            break;
         case 1:
            SSPCONbits.SSPM3  = 0;
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 1;
            
            break;
         case 2:
            SSPCONbits.SSPM3  = 0;
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 1;
            SSPCONbits.SSPM0  = 0;
            
            break;
         case 3:
            SSPCONbits.SSPM3  = 0;
            SSPCONbits.SSPM2  = 0;
            SSPCONbits.SSPM1  = 1;
            SSPCONbits.SSPM0  = 1;
            break;  
         case 4:
            SSPCONbits.SSPM3  = 0;
            SSPCONbits.SSPM2  = 1;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 0;
            
            break;
         case 5:
            SSPCONbits.SSPM3  = 0;
            SSPCONbits.SSPM2  = 1;
            SSPCONbits.SSPM1  = 0;
            SSPCONbits.SSPM0  = 1;
            break;   
    }
    
    switch (SCK)
    {
        case 0:
            SSPCONbits.CKP  = 0;
            SSPSTATbits.CKE = 0;
            
            break;
         case 1:
            SSPCONbits.CKP  = 1;
            SSPSTATbits.CKE = 0;
            
            break;
         case 2:
            SSPCONbits.CKP  = 0;
            SSPSTATbits.CKE = 1;
            
            break;
         case 3:
            SSPCONbits.CKP  = 1;
            SSPSTATbits.CKE = 1;
            break;    
    }
    SSPSTATbits.SMP = 0;
    SSPCONbits.SSPEN = 1;
}

void SPI_Enviar (char valor)
{
    SSPBUF = valor;
}


char SPI_Recibir ()
{
    return SSPBUF;
}

