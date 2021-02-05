#include "ADC_lib.h"

void initADC (uint8_t CHS)
{
    switch (CHS)
    {
        case 0: //AN0
            ADCON0bits.CHS3  = 0;        
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 0;
            break;
        case 1: //AN1
            ADCON0bits.CHS3  = 0;       
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 1;
            break;
            
        case 2: //AN2
            ADCON0bits.CHS3  = 0;       
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 1;
            ADCON0bits.CHS0  = 0;
            break;
            
        case 3: //AN3
            ADCON0bits.CHS3  = 0;       
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 1;
            ADCON0bits.CHS0  = 1;
            break;
            
        case 4: //AN4
            ADCON0bits.CHS3  = 0;        
            ADCON0bits.CHS2  = 1;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 0;
            break;
            
        case 5: //AN5
            ADCON0bits.CHS3  = 0;        
            ADCON0bits.CHS2  = 1;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 1;
            break;
            
        case 6: //AN6
            ADCON0bits.CHS3  = 0;      
            ADCON0bits.CHS2  = 1;
            ADCON0bits.CHS1  = 1;
            ADCON0bits.CHS0  = 0;
            break;
            
        case 7: //AN7
            ADCON0bits.CHS3  = 0;       
            ADCON0bits.CHS2  = 1;
            ADCON0bits.CHS1  = 1;
            ADCON0bits.CHS0  = 1;
            break;
            
        case 8: //AN8
            ADCON0bits.CHS3  = 1;      
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 0;
            break;
            
        case 9: //AN9
            ADCON0bits.CHS3  = 1;      
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 1;
            break;
            
        case 10: //AN10
            ADCON0bits.CHS3  = 1;        
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 1;
            ADCON0bits.CHS0  = 0;
            break;
            
        case 11: //AN11
            ADCON0bits.CHS3  = 1;       
            ADCON0bits.CHS2  = 0;
            ADCON0bits.CHS1  = 1;
            ADCON0bits.CHS0  = 1;
            break;
        
        case 12: //AN12
            ADCON0bits.CHS3  = 1;     
            ADCON0bits.CHS2  = 1;
            ADCON0bits.CHS1  = 0;
            ADCON0bits.CHS0  = 0;
            break;
            
        case 13: //AN13
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
    ADCON0bits.ADCS1 = 1;       //clock
    ADCON0bits.ADCS0 = 0;
    ADCON0bits.GO_DONE= 0;          //conversion apagada
    ADCON0bits.ADON  = 1;
    
    ADCON1bits.ADFM  = 1;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
}
