/* 
 * Project: Interrupciones y Librerias
 * File:    multiplex_lib.c
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 9, 2021,
 */

#include "multiplex_lib.h"                 // Se incluye header   

uint8_t adc_value = 0;                     // Variables de libreria
uint8_t show;
uint8_t unidad;
uint8_t decena;
uint8_t toogle;

void division (void)                       // Funcion division
{
    PORTEbits.RE0 = 0;                     // Se inicia los transistores en cero tener display limpio
    PORTEbits.RE1 = 0;
    
    decena = adc_value/16;                 // Se obtiene el valor de la decena del numero hexadecimal
    unidad = adc_value%16;                 // Se obtiene el valor de la unidad con el residuo
    
    if (toogle == 0)                       // Si estamos en el momento de mostrar decena
    {
        show = decena;                     // El valor de la variable es la decena
        hexadecimal (show);                // Se muestra en el display la variable
        PORTEbits.RE0 = 1;                 // Se enciende el transistor para que la variable se vea
    }
    else
    {
        show = unidad;                     // El valor de la variable es la unidad
        hexadecimal (show);                // Se muestra en el display la variable
        PORTEbits.RE1 = 1;                 // Se enciende el transistor para que la variable se vea
    }
}

void multiplexar (void)
{
    if (toogle == 0)                        // Funcion para cambiar estado de transistores
    {
        toogle = 1;                         // Si es cero ahora es uno
    }
    else
    {
        toogle = 0;                         // Si es uno ahora es cero
    }
}

void hexadecimal (uint8_t show)
{
    switch (show)                           // Menu para mostrar hexadecimal en display
    {
        case 0:                             //0
            PORTC = 0b00111111;        
            break;
        case 1:                             //1
            PORTC = 0b00000110; 
            break;
        case 2:                             //2
            PORTC = 0b01011011; 
            break; 
        case 3:                             //3
            PORTC = 0b01001111; 
            break;  
        case 4:                             //4
            PORTC = 0b01100110;
            break;
        case 5:                             //5
            PORTC = 0b01101101;
            break;  
        case 6:                             //6
            PORTC = 0b01111101;
            break; 
        case 7:                             //7
            PORTC = 0b00000111;
            break; 
        case 8:                             //8
            PORTC = 0b01111111;
            break;  
        case 9:                             //9
            PORTC = 0b01101111;
            break;   
        case 10:                            //A
            PORTC = 0b01110111;
            break;   
        case 11:                            //B
            PORTC = 0b01111100;
            break;
        case 12:                            //C
            PORTC = 0b00111001;
            break;
        case 13:                            //D
            PORTC = 0b01011110;
            break;
        case 14:                            //E
            PORTC = 0b01111001;
            break;
        case 15:                            //F
            PORTC = 0b01110001;
            break;
        default:
            PORTC = 0b00000000;
            break;
    }
}
