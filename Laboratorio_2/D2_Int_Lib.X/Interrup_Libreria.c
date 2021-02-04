/* 
 * Project: Juego de Carrera
 * File:    Juego Carrera.c
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: January 24, 2021, 7:25 PM
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

uint16_t contador = 0;                 // unsigned int counter;  uint16_t;  
uint16_t pressed_ok = 0;             //Jugador 1 variables tipo entero
uint16_t released_ok = 0;
uint16_t presionado = 0;

uint16_t contador2 = 0;                 // unsigned int counter;  uint16_t;  
uint16_t pressed_ok2 = 0;             //Jugador 1 variables tipo entero
uint16_t released_ok2 = 0;
uint16_t presionado2 = 0;

uint16_t i = 0;
uint8_t adc_value = 0;

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

//============================================================================*/
// INTERRUPCIONES
//============================================================================*/

void __interrupt() ISR(void) 
{
    //INTCONbits.GIE = 0;
    //if (INTCONbits.T0IF == 1)      
    
    
    if (INTCONbits.TMR0IF == 1)
    {
        INTCONbits.TMR0IF = 0;
        TMR0 = 150;
    }
  
    
     if (INTCONbits.RBIF == 1)                 // Verifica que el boton este presionado 
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
            PORTC = ADRESL;
            adc_value = ADRESL;
        }
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
    //INTCON = 0b10100000;       // activa interrupciones y TMR0
    INTCONbits.GIE = 1;          // Interrupt enable or ei(); 
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
    //OSCCON = 0b01110010;      
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
    //OPTION_REG = 0b00001000;
    OPTION_REGbits.nRBPU = 1;   // Activa el timer 0 en prescaler
    OPTION_REGbits.T0CS = 0; 
    OPTION_REGbits.PSA = 1;
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 0;
    TMR0 = 150;
}

void adc_config (void)
{
    initADC (0);               // En libreria
}

//============================================================================*/
// FUNCIONES
//============================================================================*/

void incrementar(void) 
{
    //*************************************************************************
    // Incremento de contador de decadas al presionar un push button
    // el cual esta en pull up y tiene anti-rebote
    //*************************************************************************
  
    if (PORTBbits.RB0 == 1)                 // Verifica que el boton este presionado 
    {
        for (int e = 0; e < 201; e++){
        pressed_ok = pressed_ok + 1; }       // Se incrementa contador que verifica que el boton este presionado con rango de seguridad 
        released_ok = 0;                    // Variable de boton libre se reduce a cero porque boton se esta presionando
        if (pressed_ok > 200)               // Si el boton esta seguramente presionado
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
        for (int e = 0; e < 201; e++){
        released_ok = released_ok + 1;} 
              // Se incrementa contador que verifica que el boton este libre con rango de seguridad 
        pressed_ok = 0;                     // Contador de boton presionado se reduce a cero porque boton esta libre
        if (released_ok > 200)              // Verifica que el boton este libre ...
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
        for (int e = 0; e < 201; e++){
        pressed_ok2 = pressed_ok2 + 1; }       // Se incrementa contador que verifica que el boton este presionado con rango de seguridad 
        released_ok2 = 0;                    // Variable de boton libre se reduce a cero porque boton se esta presionando
        if (pressed_ok2 > 200)               // Si el boton esta seguramente presionado
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
        for (int e = 0; e < 201; e++){
        released_ok2 = released_ok2 + 1;} 
              // Se incrementa contador que verifica que el boton este libre con rango de seguridad 
        pressed_ok2 = 0;                     // Contador de boton presionado se reduce a cero porque boton esta libre
        if (released_ok2 > 200)              // Verifica que el boton este libre ...
        {
            presionado2 = 0;                 // Coloca el boton como libre para siguiente ciclo
            released_ok = 0;                // Variable de boton libre se reduce a cero para siguiente ciclo
        }
    }
    return;
}