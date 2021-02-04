
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_lib_H
#define	ADC_lib_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void initADC (uint8_t CHS);

#endif	/* ADC_lib_H */

