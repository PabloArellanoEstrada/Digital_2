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
int contador = 0;                 // unsigned int counter;  uint16_t;  
//uint8_t contador;                 // unsigned int counter;  uint16_t; 

int pressed_ok = 0;             //Jugador 1 variables tipo entero
int released_ok = 0;
int presionado = 0;
int i = 0;

//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void setup(void);
void osc_config (void);
void interrup_config (void);
void tmr0_config(void);
void incrementar(void);
void decrementar(void);


//============================================================================*/
// CICLO PRINCIPAL
//============================================================================*/

void main(void) 
{
    setup();
    osc_config();
    interrup_config();
    tmr0_config();
    //**************************************************************************
    // Loop principal
    //**************************************************************************

    while (1) 
    {
          //PORTD  = contador; 
    }
}

//============================================================================*/
// INTERRUPCIONES
//============================================================================*/

void __interrupt() isr(void) 
{
    // GIE = 0; por defecto
    //INTCONbits.GIE = 0;
    //if (INTCONbits.T0IF == 1)               // Interrupcion o Timer
    //{
    //    PORTC = PORTC + 1;
    //    TMR0 = 150;   
    //    INTCONbits.T0IF = 0; 
    //}
    
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
// CONFIGURACION
//============================================================================*/

void setup(void) 
{
    
    ANSEL = 1;                // Puerto A analogico
    TRISA = 1;                 // Puerto A como entrada analogica
    PORTA = 0;                // Puerto A entrada apagado
    ANSELH = 0;               // Puerto B digital
    TRISB = 0b11111111;       // salida B RB0 y RB1 para botones y los demas de salida
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
    INTCONbits.PEIE = 1;
    INTCONbits.T0IE = 1;
    INTCONbits.INTE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.T0IF = 0;
    INTCONbits.INTF = 0;
    INTCONbits.RBIF = 0; 
    IOCB = 0b11111111;
}

void osc_config (void) 
{
    //OSCCON = 0b01110010;       // Oscilador interno a 8MHz
    OSCCONbits.IRCF  = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
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
        for (int e = 0; e < 501; e++){
        pressed_ok = pressed_ok + 1; }       // Se incrementa contador que verifica que el boton este presionado con rango de seguridad 
        released_ok = 0;                    // Variable de boton libre se reduce a cero porque boton se esta presionando
        if (pressed_ok > 500)               // Si el boton esta seguramente presionado
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
        for (int e = 0; e < 501; e++){
        released_ok = released_ok + 1;} 
              // Se incrementa contador que verifica que el boton este libre con rango de seguridad 
        pressed_ok = 0;                     // Contador de boton presionado se reduce a cero porque boton esta libre
        if (released_ok > 500)              // Verifica que el boton este libre ...
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
        for (int e = 0; e < 501; e++){
        pressed_ok = pressed_ok + 1; }       // Se incrementa contador que verifica que el boton este presionado con rango de seguridad 
        released_ok = 0;                    // Variable de boton libre se reduce a cero porque boton se esta presionando
        if (pressed_ok > 500)               // Si el boton esta seguramente presionado
        {
            if (presionado == 0)            // Verifica que el boton esta en posicion presionado
            {    
                PORTD = PORTD - 1;          // Incrementa el contador de decada del puerto
                presionado = 1;             // Coloca el boton como ya presionado para no volver a repetir este ciclo
            }
        }
        pressed_ok = 0;                     // Se reduce contador de boton de seguridad presionado para siguiente ciclo
        }
    else                                    // Si el boton no esta presionado
    {
        for (int e = 0; e < 501; e++){
        released_ok = released_ok + 1;} 
              // Se incrementa contador que verifica que el boton este libre con rango de seguridad 
        pressed_ok = 0;                     // Contador de boton presionado se reduce a cero porque boton esta libre
        if (released_ok > 500)              // Verifica que el boton este libre ...
        {
            presionado = 0;                 // Coloca el boton como libre para siguiente ciclo
            released_ok = 0;                // Variable de boton libre se reduce a cero para siguiente ciclo
        }
    }
    return;
}