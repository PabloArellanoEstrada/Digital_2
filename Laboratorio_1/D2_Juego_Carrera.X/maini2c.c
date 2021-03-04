/* 
 * Project: D2_SPI_MAESTRO
 * File:    MAESTRO_SPI.c
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






//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void setup(void);                    // Configuracion inicial
void osc_config (void);
void interrup_config (void);
void tmr0_config (void);




//============================================================================*/
// INTERRUPCIONES
//============================================================================*/

void __interrupt() ISR(void)    
{                                    // GIE = 0
    if (INTCONbits.TMR0IF == 1)      // Desboradmiento? La bandera se levanta y se revisa
    {
        INTCONbits.TMR0IF = 0;       // Se apaga la bandera manualmente
        TMR0 = 100;                
    }                                // GIE = 1
}                                      

//============================================================================*/
// CICLO PRINCIPAL
//============================================================================*/

void main(void) 
{
    setup();                             // Funciones de Configuracion
    osc_config();
    interrup_config();
    tmr0_config ();
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 1;
    SSPCONbits.SSPEN = 1;
    
    SSPCONbits.SSPM = 8;      // hight speed
    SSPADD = 9;    
    
    SSPCON2bits.SEN = 1;
    
    while(SSPCON2bits.SEN == 1)
    SSPBUF = 0xA0;
    
    while(SSPSTATbits.BF);        // limpio?
    while(SSPSTATbits.R_nW);      // escribir
    
    SSPBUF = 0x00;
    
    while(SSPSTATbits.BF);        // limpio?
    while(SSPSTATbits.R_nW);      // escribir
    
    SSPBUF = 0x64;
    
    while(SSPSTATbits.BF);        // limpio?
    while(SSPSTATbits.R_nW);      // escribir
    
    SSPBUF = 0x20;
    
    while(SSPSTATbits.BF);        // limpio?
    while(SSPSTATbits.R_nW);      // escribir
    
    SSPBUF = 0x03;
    
    while(SSPSTATbits.BF);        // limpio?
    while(SSPSTATbits.R_nW);      // escribir
    
    SSPCON2bits.PEN = 1;
    while (SSPCON2bits.PEN == 1);
        
    while (1)                           
    {
               
   
    }
}

//============================================================================*/
// CONFIGURACION
//============================================================================*/

void setup(void) 
{
    ANSEL = 0;                // Puerto A digital 
    TRISA = 0;                // Puerto A como salida 
    PORTA = 0;                // Puerto A entrada apagado
    ANSELH = 0;               // Puerto B digital
    TRISB = 0;                // Puerto B salida
    PORTB = 0;                // Puerto B apagado
    TRISC = 0;                // Puerto C salida
    PORTC = 0;                // Puerto C apagado
    TRISCbits.TRISC6 = 0;     // TX salida
    TRISCbits.TRISC4 = 1;     // SPI esclavo entrada
    PORTCbits.RC0 = 1;        // SS POT
    PORTCbits.RC1 = 1;        // SS BOTONES
    PORTCbits.RC2 = 1;        // SS SEMAFORO
    TRISD = 0;                // Puerto D salida 
    PORTD = 0;                // Puerto D apagados
    TRISE = 0;                // Puerto E salida
    PORTE = 0;                // Puerto E apagado
}

void interrup_config (void) 
{
    INTCONbits.GIE = 1;       // Interrupciones globales habilitadas
    INTCONbits.PEIE = 1;      // Interrupciones periferias habilidatas
    INTCONbits.T0IE = 1;      // Interrupcion del Timer0 habilitadas
    INTCONbits.INTE = 0;      // Interrupcion externa INT deshabilitada
    INTCONbits.RBIE = 0;      // Interrupcion del Puerto B deshabilitada
    INTCONbits.T0IF = 0;      // Bandera de Interrupcion del Timer 0
    INTCONbits.INTF = 0;      // Bandera de interrupcion del INT
    INTCONbits.RBIF = 0;      // Bandera de interrrupcion del Puerto B
    IOCB = 0b00000000;        // Interrup on Change enable apagado
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
    OPTION_REGbits.nRBPU = 0; // PORTB pull-ups habilitados
    OPTION_REGbits.T0CS = 0;  // TIMER0 como temporizador, no contador
    OPTION_REGbits.PSA = 0;   // Modulo de TIMER con prescaler, no se usa WDT
    OPTION_REGbits.PS2 = 0;   // Prescaler en 8
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 0;
    TMR0 = 100;                // Valor del TIMER0
}

//============================================================================*/
    // CONFIGURACION CON LIBRERIA
//============================================================================*/



//============================================================================*/
// FUNCIONES
//============================================================================*/

