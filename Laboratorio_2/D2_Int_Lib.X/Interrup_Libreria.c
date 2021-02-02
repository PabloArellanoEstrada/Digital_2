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

//============================================================================*/
// PALABRA DE CONFIGURACION
//============================================================================*/
// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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

//char counter = 0;               // unsigned char counter; uint8_t;
//int w_temp = 0;                 // unsigned int counter;  uint16_t;  
//uint8_t contador;                 // unsigned int counter;  uint16_t; 

//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void setup(void);
void osc_config (void);
void interrup_config (void);
void tmr0_config(void);


//============================================================================*/
// CICLO PRINCIPAL
//============================================================================*/

void main(void) {
    setup();
    osc_config();
    interrup_config();
    tmr0_config();
    //**************************************************************************
    // Loop principal
    //**************************************************************************

    while (1) {
             
    }
}

//============================================================================*/
// INTERRUPCIONES
//============================================================================*/

void __interrupt() isr(void) 
{
    INTCONbits.GIE = 0;
    if (INTCONbits.T0IE && INTCONbits.T0IF)               // Interrupcion o Timer
    {
        PORTC = PORTC + 1;
        TMR0 = 1;   
        INTCONbits.T0IF = 0; 
    }
    
     if (RBIF == 1)
    {
        RBIF = 0;
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
    PORTE = 1;                // Puerto E salida apagado
}

//============================================================================*/
// FUNCIONES
//============================================================================*/

void interrup_config (void) 
{
    INTCON = 0b10100000;       // activa interrupciones y TMR0
    INTCONbits.GIE = 1;        // Interrupt enable or ei();   
}

void osc_config (void) 
{
    OSCCON = 0b01110010;       // Oscilador interno a 8MHz
}

void tmr0_config (void) 
{
    OPTION_REG = 0b10000100;   // Activa el timer 0 en prescaler
    TMR0 = 1;
}
