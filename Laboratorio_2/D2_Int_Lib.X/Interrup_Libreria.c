/* 
 * Project: Interrupciones y Librerias
 * File:    Interrupciones y Librerias.c
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 9, 2021,
 */

//============================================================================*/
// LIBRERIAS
//============================================================================*/

#include <xc.h>                 // 
#include <stdint.h>             // Variables de ancho definico
#include <stdio.h>             // Variables de ancho definico
#include "ADC_lib.h"

//============================================================================*/
// PALABRA DE CONFIGURACION
//============================================================================*/
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT         // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// DEFINE
#define _XTAL_FREQ 8000000
//#define LED_rojo PORTEbits.RE0

//============================================================================*/
// VARIABLES
//============================================================================*/

uint16_t contador = 0;               
uint16_t pressed_ok = 0;           
uint16_t released_ok = 0;
uint16_t presionado = 0;

uint16_t contador2 = 0;                
uint16_t pressed_ok2 = 0;            
uint16_t released_ok2 = 0;
uint16_t presionado2 = 0;

uint16_t i = 0;
uint8_t adc_value = 0;
uint8_t timer_contador = 0;
uint8_t show = 0;
uint8_t unidad;
uint8_t decena;
uint8_t toogle;

//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void setup(void);
void osc_config (void);
void interrup_config (void);
void tmr0_config(void);
void incrementar(void);
void decrementar(void);
void adc_config (void);
void multiplexar (void);
void division (void);
void hexadecimal();
void big (void);

//============================================================================*/
// INTERRUPCIONES
//============================================================================*/

void __interrupt() ISR(void) 
{
    //INTCONbits.GIE = 0;
    if (INTCONbits.TMR0IF == 1)
    {
        INTCONbits.TMR0IF = 0;
        TMR0 = 10;
        division ();
       
    }
  
     if (INTCONbits.RBIF == 1)                 
    {
        uint8_t  a;
        a = PORTB; 
        incrementar();
        decrementar();
        INTCONbits.RBIF = 0;
    }
    // GIE = 1;por defecto
 }

//============================================================================*/
// CICLO PRINCIPAL
//============================================================================*/

void main(void) 
{
    setup();
    osc_config();
    interrup_config();
    tmr0_config();
    adc_config ();
  
    
    //**************************************************************************
    // Loop principal
    //**************************************************************************
   
     while (1) 
    {
        ADCON0bits.GO_DONE = 1; 
        __delay_ms(10);
        if (ADCON0bits.GO_DONE == 0)
        {
            ADCON0bits.GO_DONE = 1;
            adc_value = ADRESL;
        }
        big();
    }
}


    
//============================================================================*/
// CONFIGURACION
//============================================================================*/

void setup(void) 
{
    
    ANSEL = 1;                // Puerto A analogico
    TRISA = 1;                 // Puerto A como entrada analogica
    PORTA = 0;                // Puerto A entrada apagado
    ANSELH = 0;               // Puerto B digital
    TRISB = 0b00000011;       // salida B RB0 y RB1 para botones y los demas de salida
    PORTB = 0;                // Puerto B RB0 y RB1 entrada igual a 0
    TRISC = 0;                // Puerto C salida leds
    PORTC = 0;                // Puerto C salida leds apagados
    TRISD = 0;                // Puerto D salida display
    PORTD = 0;                // Puerto D salida display apagados
    TRISE = 0;                // Puerto E salida transistores y alarma
    PORTE = 0;                // Puerto E salida apagado
}

void interrup_config (void) 
{
    INTCONbits.GIE = 1;          
    INTCONbits.PEIE = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.INTE = 0;
    INTCONbits.RBIE = 1;
    INTCONbits.T0IF = 0;
    INTCONbits.INTF = 0;
    INTCONbits.RBIF = 0; 
    IOCB = 0b00000011;
}

void osc_config (void) 
{  
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.OSTS = 0;
    OSCCONbits.HTS = 0;
    OSCCONbits.LTS = 1;
    OSCCONbits.SCS = 0;
}

void tmr0_config (void) 
{
    OPTION_REGbits.nRBPU = 1;   
    OPTION_REGbits.T0CS = 0; 
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 0;
    TMR0 = 10;
}

void adc_config (void)
{
    initADC (0);               // En libreria
}

//============================================================================*/
// FUNCIONES
//============================================================================*/

void division (void)
{
    PORTEbits.RE0 = 0;
    PORTEbits.RE1 = 0;
    
    decena = adc_value/16;
    unidad = adc_value%16;
    
    if (toogle == 0)
    {
        show = decena;
        hexadecimal (show);
        PORTEbits.RE0 = 1;
    }
    else
    {
        show = unidad;
        hexadecimal (show);
        PORTEbits.RE1 = 1;
    }
    multiplexar();
}

void multiplexar (void)
{
    if (toogle == 0)
    {
        toogle = 1;
    }
    else
    {
        toogle = 0;
    }
}

void hexadecimal (show)
{
    switch (show)
    {
        case 0: //0
            PORTC = 0b00111111;        
            break;
        case 1: //1
            PORTC = 0b00000110; 
            break;
        case 2: //2
            PORTC = 0b01011011; 
            break; 
        case 3: //3
            PORTC = 0b01001111; 
            break;  
        case 4: //4
            PORTC = 0b01100110;
            break;
        case 5: //5
            PORTC = 0b01101101;
            break;  
        case 6: //6
            PORTC = 0b01111101;
            break; 
        case 7: //7
            PORTC = 0b00000111;
            break; 
        case 8: //8
            PORTC = 0b01111111;
            break;  
        case 9: //9
            PORTC = 0b01101111;
            break;   
        case 10: //A
            PORTC = 0b01110111;
            break;   
        case 11: //B
            PORTC = 0b01111100;
            break;
        case 12: //C
            PORTC = 0b00111001;
            break;
        case 13: //D
            PORTC = 0b01011110;
            break;
        case 14: //E
            PORTC = 0b01111001;
            break;
        case 15: //F
            PORTC = 0b01110001;
            break;
        default:
            PORTC = 0b00000000;
            break;
    }
}

void big (void)
{
    if (adc_value > PORTD)
    {
        PORTEbits.RE2 = 1;
    }
    else
    {
        PORTEbits.RE2 = 0;
    }
}

void incrementar(void) 
{
    //*************************************************************************
    // Incremento de contador de decadas al presionar un push button
    // el cual esta en pull up y tiene anti-rebote
    //*************************************************************************
  
    if (PORTBbits.RB0 == 1)                 // Verifica que el boton este presionado 
    {
        for (int e = 0; e < 11; e++){
        pressed_ok = pressed_ok + 1; }       // Se incrementa contador que verifica que el boton este presionado con rango de seguridad 
        released_ok = 0;                    // Variable de boton libre se reduce a cero porque boton se esta presionando
        if (pressed_ok > 10)               // Si el boton esta seguramente presionado
        {
            if (presionado == 0)            // Verifica que el boton esta en posicion presionado
            {    
                PORTD = PORTD + 1;          // Incrementa el contador de decada del puerto
                presionado = 1;             // Coloca el boton como ya presionado para no volver a repetir este ciclo
            }
        }
        pressed_ok = 0;                     // Se reduce contador de boton de seguridad presionado para siguiente ciclo
        }
    else                                    // Si el boton no esta presionado
    {
        for (int e = 0; e < 11; e++){
        released_ok = released_ok + 1;} 
              // Se incrementa contador que verifica que el boton este libre con rango de seguridad 
        pressed_ok = 0;                     // Contador de boton presionado se reduce a cero porque boton esta libre
        if (released_ok > 10)              // Verifica que el boton este libre ...
        {
            presionado = 0;                 // Coloca el boton como libre para siguiente ciclo
            released_ok = 0;                // Variable de boton libre se reduce a cero para siguiente ciclo
        }
    }
    return;
}

void decrementar(void) 
{
    //*************************************************************************
    // Incremento de contador de decadas al presionar un push button
    // el cual esta en pull up y tiene anti-rebote
    //*************************************************************************
  
    if (PORTBbits.RB1 == 1)                 // Verifica que el boton este presionado 
    {
        for (int e = 0; e < 11; e++){
        pressed_ok2 = pressed_ok2 + 1; }       // Se incrementa contador que verifica que el boton este presionado con rango de seguridad 
        released_ok2 = 0;                    // Variable de boton libre se reduce a cero porque boton se esta presionando
        if (pressed_ok2 > 10)               // Si el boton esta seguramente presionado
        {
            if (presionado2 == 0)            // Verifica que el boton esta en posicion presionado
            {    
                PORTD = PORTD - 1;          // Incrementa el contador de decada del puerto
                presionado2 = 1;             // Coloca el boton como ya presionado para no volver a repetir este ciclo
            }
        }
        pressed_ok2 = 0;                     // Se reduce contador de boton de seguridad presionado para siguiente ciclo
        }
    else                                    // Si el boton no esta presionado
    {
        for (int e = 0; e < 11; e++){
        released_ok2 = released_ok2 + 1;} 
              // Se incrementa contador que verifica que el boton este libre con rango de seguridad 
        pressed_ok2 = 0;                     // Contador de boton presionado se reduce a cero porque boton esta libre
        if (released_ok2 > 10)              // Verifica que el boton este libre ...
        {
            presionado2 = 0;                 // Coloca el boton como libre para siguiente ciclo
            released_ok = 0;                // Variable de boton libre se reduce a cero para siguiente ciclo
        }
    }
    return;
}