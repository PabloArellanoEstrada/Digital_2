/* 
 * Project: D2_SLAVE_POT
 * File:    SLAVE_POT.c
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 18, 2021.
 */

//============================================================================*/
// LIBRERIAS
//============================================================================*/

#include <xc.h>                 // XC8 libreria
#include <stdint.h>             // Variables de ancho definido
#include <stdio.h>              // Tipos de variables, macros, entradas y salidas
#include "ADC_SPI.h"            // Libreria Personalizada ADC
#include "SPI_SPI.h"            // Libreria comunicacion SPI

//============================================================================*/
// PALABRA DE CONFIGURACION
//============================================================================*/
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscilador interno
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
#define _XTAL_FREQ 8000000      // Frecuencia compilador

//============================================================================*/
// VARIABLES
//============================================================================*/

char dato_maestro;               // Dato de maestro

//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void setup(void);                // Configuracion inicial
void osc_config (void);
void interrup_config (void);
void tmr0_config (void);
void SPI_config (void);
void adc_config (void);

void adc_conversion (void);      // Conversion

//============================================================================*/
// INTERRUPCIONES
//============================================================================*/

void __interrupt() ISR(void)    
{                                // GIE = 0
    if (INTCONbits.TMR0IF == 1)  // Desbordamiento? la bandera se levanta y se revisa
    {
        adc_conversion();        // Conversion 
        INTCONbits.TMR0IF = 0;   // Se apaga la bandera manualmente
        TMR0 = 100;                
    }                            // GIE = 1
}                            

//============================================================================*/
// CICLO PRINCIPAL
//============================================================================*/

void main(void) 
{
    setup();                            // Funciones de Configuracion
    osc_config();
    interrup_config();
    tmr0_config ();
    adc_config();
    SPI_config ();
    
    while (1)                           // Loop principal
    {
        if (SSPIF == 1)                 // Bandera levantada?
        {
        dato_maestro = SPI_Recibir();   // Se recibe dato de maestro
        SPI_Enviar (ADRESH);            // Se envia conversion
        SSPIF = 0;                      // Se apaga bandera
        }
    }
}

//============================================================================*/
// CONFIGURACION
//============================================================================*/

void setup(void) 
{
    ANSEL = 0;                // Puerto A digital
    TRISA = 0;                // Puerto A entrada
    PORTA = 0;                // Puerto A apagado
    TRISAbits.TRISA0 = 1;     // A0 entrada
    ANSELbits.ANS0 = 1;       // A0 analogico Potenciometro
    TRISAbits.TRISA5 = 1;     // A5 entrada
    ANSELbits.ANS5 = 0;       // A5 digital
    PORTAbits.RA5 = 1;        // A5 SS deshabilitado
    
    ANSELH = 0;               // Puerto B digital
    TRISB = 0;                // Puerto B salida
    PORTB = 0;                // Puerto B apagado
    TRISC = 0;                // Puerto C salida 
    PORTC = 0;                // Puerto C apagado
    TRISCbits.TRISC3 = 1;     // Entra Reloj de Maestro
    TRISCbits.TRISC4 = 1;     // Entra dato de Maestro
    TRISCbits.TRISC5 = 0;     // Sale dato a Esclavo
    TRISD = 0;                // Puerto D salida 
    PORTD = 0;                // Puerto D apagados
    TRISE = 0;                // Puerto E salida
    PORTE = 0;                // Puerto E apagado
}

void interrup_config (void) 
{
    INTCONbits.GIE = 1;       // Interrupciones globales habilitadas
    INTCONbits.PEIE = 1;      // Interrupciones periferias habilidatas
    INTCONbits.T0IE = 1;      // Interrupcion del Timer0 habilitada
    INTCONbits.INTE = 0;      // Interrupcion externa INT deshabilitada
    INTCONbits.RBIE = 0;      // Interrupcion del Puerto B habilitadas
    INTCONbits.T0IF = 0;      // Bandera de Interrupcion del Timer 0
    INTCONbits.INTF = 0;      // Bandera de interrupcion del INT
    INTCONbits.RBIF = 0;      // Bandera de interrrupcion del Puerto B
    IOCB = 0b00000000;        // Interrup on Change enable deshabilitado
}

void osc_config (void) 
{  
    OSCCONbits.IRCF2 = 1;     // Oscilador en 4Mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;     
    OSCCONbits.OSTS  = 0;     // Oscilador interno
    OSCCONbits.HTS   = 0;       
    OSCCONbits.LTS   = 1;
    OSCCONbits.SCS   = 0;     // Oscilador basado en el reloj
}

void tmr0_config (void) 
{
    OPTION_REGbits.nRBPU = 1;  // PORTB pull-ups habilitados
    OPTION_REGbits.T0CS = 0;   // TIMER0 como temporizador, no contador
    OPTION_REGbits.PSA = 0;    // Modulo de TIMER con prescaler, no se usa WDT
    OPTION_REGbits.PS2 = 0;    // Prescaler en 8
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 0;
    TMR0 = 100;                // Valor del TIMER0
}

//============================================================================*/
    // CONFIGURACION CON LIBRERIA
//============================================================================*/

void adc_config (void)
{
    initADC (0);              // ADC en canal 0
}

void SPI_config (void)
{
    SPI_Esclavo_Init (4, 2);  // Maestro con Port_Mode/16 y con 
}

//============================================================================*/
// FUNCIONES
//============================================================================*/

void adc_conversion (void)
{
    ADCON0bits.GO_DONE = 1;           // GO_DONE para iniciar conversion
    __delay_ms(10);                   // Se da tiempo para el Acquisition Time Example
    if (ADCON0bits.GO_DONE == 0)      // Si ya termino la conversion
    {
        ADCON0bits.GO_DONE = 1;       // Se inicia el GO_DONE para iniciar nuevamente
        PORTD = ADRESH * 2;           // Registro del doble de la Registro en LEDS 
        
    }
}




