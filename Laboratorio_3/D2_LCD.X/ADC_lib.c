/* 
 * Project: Interrupciones y Librerias
 * File:    ADC_lib.c
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 9, 2021,
 */

#include "ADC_lib.h"                    // Se incluye header

void initADC (uint8_t CHS)
{
    switch (CHS)                        // Menu para elegir canal
    {
        case 0:                         //AN0
            ADCON0bits.CHS3  = 0;        
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 0;
            break;
        case 1:                         //AN1
            ADCON0bits.CHS3  = 0;       
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 1;
            break;
            
        case 2:                         //AN2
            ADCON0bits.CHS3  = 0;       
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 1;
            ADCON0bits.CHS0  = 0;
            break;
            
        case 3:                         //AN3
            ADCON0bits.CHS3  = 0;       
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 1;
            ADCON0bits.CHS0  = 1;
            break;
            
        case 4:                         //AN4
            ADCON0bits.CHS3  = 0;        
            ADCON0bits.CHS2  = 1;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 0;
            break;
            
        case 5:                         //AN5
            ADCON0bits.CHS3  = 0;        
            ADCON0bits.CHS2  = 1;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 1;
            break;
            
        case 6:                         //AN6
            ADCON0bits.CHS3  = 0;      
            ADCON0bits.CHS2  = 1;
            ADCON0bits.CHS1  = 1;
            ADCON0bits.CHS0  = 0;
            break;
            
        case 7:                         //AN7
            ADCON0bits.CHS3  = 0;       
            ADCON0bits.CHS2  = 1;
            ADCON0bits.CHS1  = 1;
            ADCON0bits.CHS0  = 1;
            break;
            
        case 8:                         //AN8
            ADCON0bits.CHS3  = 1;      
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 0;
            break;
            
        case 9:                         //AN9
            ADCON0bits.CHS3  = 1;      
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 1;
            break;
            
        case 10:                        //AN10
            ADCON0bits.CHS3  = 1;        
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 1;
            ADCON0bits.CHS0  = 0;
            break;
            
        case 11:                        //AN11
            ADCON0bits.CHS3  = 1;       
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 1;
            ADCON0bits.CHS0  = 1;
            break;
        
        case 12:                        //AN12
            ADCON0bits.CHS3  = 1;     
            ADCON0bits.CHS2  = 1;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 0;
            break;
            
        case 13:                        //AN13
            ADCON0bits.CHS3  = 1;       
            ADCON0bits.CHS2  = 1;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 1;
            break;
            
        default:
            ADCON0bits.CHS3  = 1;      
            ADCON0bits.CHS2  = 1;
            ADCON0bits.CHS1  = 1;
            ADCON0bits.CHS0  = 0;
            break;
    }
    ADCON0bits.ADCS1 = 1;               // Frecuencia de Oscilacion / 32
    ADCON0bits.ADCS0 = 0;
    ADCON0bits.GO_DONE= 0;              // Conversion apagada al principio
    ADCON0bits.ADON  = 1;               // La conversion esta habilitada
    
    ADCON1bits.ADFM  = 1;               // Justificado a la derecha 
    ADCON1bits.VCFG1 = 0;               // Voltaje = 5V
    ADCON1bits.VCFG0 = 0;               // Tierra  = 0V
}

