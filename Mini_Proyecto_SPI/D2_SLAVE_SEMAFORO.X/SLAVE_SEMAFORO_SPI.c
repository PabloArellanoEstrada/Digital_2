/* 
 * Project: LCD
 * File:    LCD.c
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: February 8, 2021,
 */

//============================================================================*/
// LIBRERIAS
//============================================================================*/

#include <xc.h>                 // XC8 libreria
#include <stdint.h>             // Variables de ancho definido
#include <stdio.h>              // Tipos de variables, macros, entradas y salidas
#include "SPI_SPI.h"
#include "ADC_SPI.h" 


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
#define _XTAL_FREQ 8000000
#define LED_rojo     PORTBbits.RB0  // Uso de defines para mejor identificacion de color de semaforo
#define LED_amarillo PORTBbits.RB1  
#define LED_verde    PORTBbits.RB2

//============================================================================*/
// VARIABLES
//============================================================================*/

char dato;
char SPI_value;
char dato_maestro;
char valor1;
char valor2;
unsigned int temp;

//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void setup(void);                    // Configuracion inicial
void osc_config (void);
void interrup_config (void);
void tmr0_config (void);
void SPI_config (void);
void adc_config (void);
void adc_conversion (void);
void semaforo(void); 

//============================================================================*/
// INTERRUPCIONES
//============================================================================*/

void __interrupt() ISR(void)    
{                               // GIE = 0
    if (INTCONbits.TMR0IF == 1)  // Si hay desboradmiento de TIMER0 la bandera se levanta y se revisa
    {
        adc_conversion();
        INTCONbits.TMR0IF = 0;   // Se apaga la bandera manualmente
        TMR0 = 100;                
    }
    
    
}                               // GIE = 1

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
       
        if (SSPIF == 1)
        {
         
            dato_maestro = SPI_Recibir();
            SPI_Enviar (ADRESH);
            SSPIF = 0;
        }
              
    }
}

//============================================================================*/
// CONFIGURACION
//============================================================================*/

void setup(void) 
{
    ANSEL = 0;                // Puerto A digital
    TRISA = 0;                // Puerto A como entrada
    PORTA = 0;                // Puerto A entrada apagado
    TRISAbits.TRISA0 = 1;     // Entrada
    ANSELbits.ANS0 = 1;       // Analogico
    TRISAbits.TRISA5 = 1;     // Bit 5 entrada
    ANSELbits.ANS5 = 0;       // Digital
    PORTAbits.RA5 = 1;        // Puerto A entrada apagado
    
    ANSELH = 0;               // Puerto B digital
    TRISB = 0;                // Puerto B salida
    PORTB = 0;                // Puerto B RB0 y RB1 entrada igual a 0
    LED_rojo = 0;
    LED_amarillo = 0;
    LED_verde = 0;
    TRISC = 0;                // Puerto C salida leds
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC5 = 0;
    PORTC = 0;                // Puerto C salida leds apagados
    TRISD = 0;                // Puerto D salida display
    PORTD = 0;                // Puerto D salida apagados
    TRISE = 0;                // Puerto E salida transistores y alarma
    PORTE = 0;                // Puerto E salida apagado
}

void interrup_config (void) 
{
    INTCONbits.GIE = 1;       // Interrupciones globales habilitadas
    INTCONbits.PEIE = 1;      // Interrupciones periferias habilidatas
    INTCONbits.T0IE = 1;      // Interrupcion del Timer0 deshabilitada
    INTCONbits.INTE = 0;      // Interrupcion externa INT deshabilitada
    INTCONbits.RBIE = 0;      // Interrupcion del Puerto B habilitadas
    INTCONbits.T0IF = 0;      // Bandera de Interrupcion del Timer 0
    INTCONbits.INTF = 0;      // Bandera de interrupcion del INT
    INTCONbits.RBIF = 0;      // Bandera de interrrupcion del Puerto B
    IOCB = 0b00000000;        // Interrup on Change enable
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
    OPTION_REGbits.nRBPU = 1; // PORTB pull-ups habilitados
    OPTION_REGbits.T0CS = 0;  // TIMER0 como temporizador, no contador
    OPTION_REGbits.PSA = 0;   // Modulo de TIMER con prescaler, no se usa WDT
    OPTION_REGbits.PS2 = 0;   // Prescaler en 8
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 0;
    TMR0 = 100;                // Valor del TIMER0 para un delay de 0.246 seg.
}

//============================================================================*/
    // CONFIGURACION CON LIBRERIA
//============================================================================*/

void adc_config (void)
{
    initADC (0);              // ADC
}

void SPI_config (void)
{
    SPI_Esclavo_Init (4, 2);
}

//============================================================================*/
// FUNCIONES
//============================================================================*/

void semaforo(void) 
{
    if (PORTD < 13)
    {
        LED_verde = 1;
        LED_amarillo = 0;
        LED_rojo = 0;
    }
    else if (PORTD >= 13 && PORTD <= 18)
    {
        LED_verde = 0;
        LED_amarillo = 1;
        LED_rojo = 0;
    }
    else
    {
        LED_verde = 0;
        LED_amarillo = 0;
        LED_rojo = 1;
    }
}

void adc_conversion (void)
{
    ADCON0bits.GO_DONE = 1;            // GO_DONE para iniciar conversion
    __delay_ms(10);                    // Se da tiempo para el Acquisition Time Example
    if (ADCON0bits.GO_DONE == 0)       // Si ya termino la conversion
    {
        ADCON0bits.GO_DONE = 1;        // Se inicia el GO_DONE para iniciar nuevamente
        temp = ADRESH * 1.95;
        valor1  = temp / 10;
        valor2  = temp % 10;
        PORTD = ADRESH;
        semaforo();
    }
}


