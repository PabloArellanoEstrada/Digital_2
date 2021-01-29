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

#include <xc.h>
#include <math.h>

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
#define LED_rojo PORTEbits.RE0
#define LED_amarillo PORTEbits.RE1
#define LED_verde PORTEbits.RE2

//============================================================================*/
// CONFIGURACION
//============================================================================*/

void setup(void) {
    TRISE = 0;                  //Puerto E como salida
    PORTE = 0;                  //Puerto E apagado
    ANSEL = 0;                  //Puerto A digital
    ANSELH = 0;                 //Puerto B digital
    TRISB = 0b00000111;         //Puerto B entrada para botones
    PORTB = 0;                  //Puerto B apagado
    TRISC = 0;                  //Puerto C como salida
    PORTC = 0;                  //Puerto C apagado
    TRISD = 0;                  //Puerto C como salida
    PORTD = 0;                  //Puerto C apagado
    TRISA = 0;                  //Puerto A como salida
    PORTA = 0;                  //Puerto A apagado
}

//============================================================================*/
// VARIABLES
//============================================================================*/

int pressed_ok = 0;     //Jugador 1
int released_ok = 0;
int presionado = 0;
int i = 0;

int pressed_ok2 = 0;     //Jugador 2
int released_ok2 = 0;
int presionado2 = 0;
int i2 = 0;


//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void setup(void);
void semaforo(void);
void verde (void);
void jugador_1 (void);
void jugador_2 (void);

//============================================================================*/
// CICLO PRINCIPAL
//============================================================================*/

void main(void) {
    setup();

    //**************************************************************************
    // Loop principal
    //**************************************************************************

    while (1) {
        if (PORTBbits.RB0 == 0)
        {
            if (PORTAbits.RA2 == 0)
            {
                semaforo();
                verde();
                PORTAbits.RA2 = 1;
            }
        }
        if (PORTAbits.RA0 == 0 && PORTAbits.RA1 == 0)
        {
            if (PORTAbits.RA0 == 1)
            {
                PORTBbits.RB2 = 0;
            }
            else if (PORTAbits.RA1 == 1)
            {
                PORTBbits.RB1 = 0;
            }
            else
            {
                jugador_1();
                jugador_2();
            }
        }
    }
}

//============================================================================*/
// FUNCIONES
//============================================================================*/

void semaforo(void) {
    LED_rojo = 1;
    __delay_ms(700);
    LED_rojo = 0;
    LED_amarillo = 1;
    __delay_ms(700);
    LED_amarillo = 0;
    return;      
}

void verde(void) {
    LED_verde = 1;
    LED_rojo = 0;
    LED_amarillo = 0;
}

void jugador_1(void) 
{
    if (PORTAbits.RA2 == 1)
    {
        if (PORTBbits.RB1 == 0)
        {
            pressed_ok = pressed_ok + 1;
            released_ok = 0;
            if (pressed_ok > 500)
            {
                if (presionado == 0)
                {    
                    PORTC = pow(2,i);
                    i = i + 1;
                    presionado = 1;
                    if (i == 8)
                    {
                        PORTAbits.RA0 = 1;
                    }
                }
                pressed_ok = 0;
            }
        }
        else
        {
            released_ok = released_ok + 1;
            pressed_ok = 0;
            if (released_ok > 500)
            {
                presionado = 0;
                released_ok = 0;
            }
        }
    }
}

void jugador_2(void) 
{
    if (PORTAbits.RA2 == 1)
    {
        if (PORTBbits.RB2 == 0)
        {
            pressed_ok2 = pressed_ok2 + 1;
            released_ok2 = 0;
            if (pressed_ok2 > 500)
            {
                if (presionado2 == 0)
                {    
                    PORTD = pow(2,i2);
                    i2 = i2 + 1;
                    presionado2 = 1;
                    if (i2 == 8)
                    {
                        PORTAbits.RA1 = 1;
                    }
                }
                pressed_ok2 = 0;
            }
        }
        else
        {
            released_ok2 = released_ok2 + 1;
            pressed_ok2 = 0;
            if (released_ok2 > 500)
            {
                presionado2 = 0;
                released_ok2 = 0;
            }
        }
    }
}
                  
 

