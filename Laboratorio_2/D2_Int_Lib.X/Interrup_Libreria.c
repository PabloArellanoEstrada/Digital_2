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
#include <stdint.h>             // Variables de ancho definido
#include <stdio.h>              // Variables
#include "ADC_lib.h"            // Libreria Personalizada ADC
#include "multiplex_lib.h"      // Libreria Personalizada Multiplexacion

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

//============================================================================*/
// VARIABLES
//============================================================================*/

uint16_t contador = 0;           // Variables Boton 1        
uint16_t pressed_ok = 0;           
uint16_t released_ok = 0;
uint16_t presionado = 0;

uint16_t contador2 = 0;          // Variables Boton 2        
uint16_t pressed_ok2 = 0;            
uint16_t released_ok2 = 0;
uint16_t presionado2 = 0;

uint16_t i = 0;                  // Variables Configuracion ADC
uint8_t adc_value = 0;

uint8_t show = 0;                // Variables Multiplexacion
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
void adc_config (void);
void incrementar(void);
void decrementar(void);
void multiplex_config();
void big (void);

//============================================================================*/
// INTERRUPCIONES
//============================================================================*/

void __interrupt() ISR(void) 
{
                                 // La interrupcion global GIE inicia automaticamente con GIE = 0
    if (INTCONbits.TMR0IF == 1)  // Si hay desboradmiento de TIMER0 la bandera se levanta y se revisa
    {
        INTCONbits.TMR0IF = 0;   // Se apaga la bandera manualmente
        TMR0 = 10;                
        multiplex_config();      // Se llama a funcion para multiplexar y mostrar
    }
  
     if (INTCONbits.RBIF == 1)   // Si se presiona un boton con Interruption on Change               
    {
        uint8_t  a;              // Para evitar Mismatch se sobre-escribe al Puerto B
        a = PORTB; 
        incrementar();           // Se llama funcion de incrementar
        decrementar();           // Se llama funcion de decrementar
        INTCONbits.RBIF = 0;     // La bandera se apaga manualmente
    }
                                 // La interrupcion global GIE finaliza automaticamente con GIE = 1 para la siguiente
 }

//============================================================================*/
// CICLO PRINCIPAL
//============================================================================*/

void main(void) 
{
    setup();                            // Funciones de Configuracion
    osc_config();
    interrup_config();
    tmr0_config();
    adc_config ();
  
    while (1)                           // LOOP PRINCIPAL **********************
    {
        ADCON0bits.GO_DONE = 1;         // Se inicia el GO_DONE para iniciar conversion
        __delay_ms(10);                 // Se da tiempo para el Acquisition Time Example
        if (ADCON0bits.GO_DONE == 0)    // Si ya termino la conversion
        {
            ADCON0bits.GO_DONE = 1;     // Se inicia el GO_DONE para iniciar nuevamente
            adc_value = ADRESL;         // Se Coloca el valor del registro de la conversion en una variable
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
    TRISA = 1;                // Puerto A como entrada analogica
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
    INTCONbits.GIE = 1;       // Interrupciones globales habilitadas
    INTCONbits.PEIE = 0;      // Interrupciones periferias deshabilidatas
    INTCONbits.T0IE = 1;      // Interrupcion del Timer0 habilitada
    INTCONbits.INTE = 0;      // Interrupcion externa INT deshabilitada
    INTCONbits.RBIE = 1;      // Interrupcion del Puerto B habilitadas
    INTCONbits.T0IF = 0;      // Bandera de Interrupcion del Timer 0
    INTCONbits.INTF = 0;      // Bandera de interrupcion del INT
    INTCONbits.RBIF = 0;      // Bandera de interrrupcion del Puerto B
    IOCB = 0b00000011;        // Interrup on Change enable
}

void osc_config (void) 
{  
    OSCCONbits.IRCF2 = 1;     // Oscilador en 4Mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.OSTS = 0;      // Oscilador interno
    OSCCONbits.HTS = 0;       
    OSCCONbits.LTS = 1;
    OSCCONbits.SCS = 0;       // Oscilador basado en el reloj
}

void tmr0_config (void) 
{
    OPTION_REGbits.nRBPU = 1; // PORTB pull-ups habilitados
    OPTION_REGbits.T0CS = 0;  // TIMER0 como temporizador, no contador
    OPTION_REGbits.PSA = 0;   // Modulo de TIMER con prescaler, no se usa WDT
    OPTION_REGbits.PS2 = 0;   // Prescaler en 8
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 0;
    TMR0 = 10;                // Valor del TIMER0 para un delay de 0.246 seg.
}

//============================================================================*/
// FUNCIONES CON LIBRERIA
//============================================================================*/

void adc_config (void)
{
    initADC (0);              // Configuracion de ADC en libreria
}

void multiplex_config()
{
    division();               // Determina unidades y decenas
    multiplexar();            // Enciende y apaga bits de transistores
    hexadecimal(show);        // Muestra el valor segun calculo anterior 
}

//============================================================================*/
// FUNCIONES
//============================================================================*/

void big (void)
{
    if (adc_value > PORTD)    // Si el numero de la conversion analogica es mayor al contador
    {
        PORTEbits.RE2 = 1;    // Enciende alarma
    }
    else
    {
        PORTEbits.RE2 = 0;    // Apaga alarma
    }
}

void incrementar(void) 
{
    if (PORTBbits.RB0 == 1)                // Verifica que el boton este presionado 
    {
        for (int e = 0; e < 11; e++){
        pressed_ok = pressed_ok + 1; }     // Se incrementa contador que verifica que el boton este presionado con rango de seguridad 
        released_ok = 0;                   // Variable de boton libre se reduce a cero porque boton se esta presionando
        if (pressed_ok > 10)               // Si el boton esta seguramente presionado
        {
            if (presionado == 0)           // Verifica que el boton esta en posicion presionado
            {    
                PORTD = PORTD + 1;         // Incrementa el contador de decada del puerto
                presionado = 1;            // Coloca el boton como ya presionado para no volver a repetir este ciclo
            }
        }
        pressed_ok = 0;                    // Se reduce contador de boton de seguridad presionado para siguiente ciclo
        }
    else                                   // Si el boton no esta presionado
    {
        for (int e = 0; e < 11; e++){
        released_ok = released_ok + 1;}    // Se incrementa contador que verifica que el boton este libre con rango de seguridad 
        pressed_ok = 0;                    // Contador de boton presionado se reduce a cero porque boton esta libre
        if (released_ok > 10)              // Verifica que el boton este libre ...
        {
            presionado = 0;                // Coloca el boton como libre para siguiente ciclo
            released_ok = 0;               // Variable de boton libre se reduce a cero para siguiente ciclo
        }
    }
    return;
}

void decrementar(void) 
{
    if (PORTBbits.RB1 == 1)                // Verifica que el boton este presionado 
    {
        for (int e = 0; e < 11; e++){
        pressed_ok2 = pressed_ok2 + 1; }   // Se incrementa contador que verifica que el boton este presionado con rango de seguridad 
        released_ok2 = 0;                  // Variable de boton libre se reduce a cero porque boton se esta presionando
        if (pressed_ok2 > 10)              // Si el boton esta seguramente presionado
        {
            if (presionado2 == 0)          // Verifica que el boton esta en posicion presionado
            {    
                PORTD = PORTD - 1;         // Decrementa el contador de decada del puerto
                presionado2 = 1;           // Coloca el boton como ya presionado para no volver a repetir este ciclo
            }
        }
        pressed_ok2 = 0;                    // Se reduce contador de boton de seguridad presionado para siguiente ciclo
        }
    else                                    // Si el boton no esta presionado
    {
        for (int e = 0; e < 11; e++){
        released_ok2 = released_ok2 + 1;}   // Se incrementa contador que verifica que el boton este libre con rango de seguridad 
        pressed_ok2 = 0;                    // Contador de boton presionado se reduce a cero porque boton esta libre
        if (released_ok2 > 10)              // Verifica que el boton este libre ...
        {
            presionado2 = 0;                // Coloca el boton como libre para siguiente ciclo
            released_ok = 0;                // Variable de boton libre se reduce a cero para siguiente ciclo
        }
    }
    return;
}