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
#include <math.h>               // Libreria matematica para usar potencia

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
#define _XTAL_FREQ 8000000      // Configuracion para usar delay de compilador en micro-segundos
#define LED_rojo PORTEbits.RE0  // Uso de defines para mejor identificacion de color de semaforo
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

int pressed_ok = 0;             //Jugador 1 variables tipo entero
int released_ok = 0;
int presionado = 0;
int i = 0;

int pressed_ok2 = 0;            //Jugador 2 variables tipo entero
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
    // Loop principal semaforo
    //**************************************************************************

    while (1) {
        if (PORTBbits.RB0 == 0)                          // Si boton de semaforo es presionado procede a encender
        {
            if (PORTAbits.RA2 == 0)                      // Verifica puerto RA2 para el jugador pueda presionar boton
            {
                semaforo();                              // Corre semaforo y luego deja encendido color verde
                verde();
                PORTAbits.RA2 = 1;                       // Enciende puerto RA2 para permitir al jugador presionar boton de juego
            }
        }
        
    //**************************************************************************
    // Loop principal ganador
    //**************************************************************************    
        
        if (PORTAbits.RA0 == 0 && PORTAbits.RA1 == 0)    // Revisa si leds de ganador todavia estan apagados para no bloquear juego
        {
            if (PORTAbits.RA0 == 1)                      // Si jugador 1 enciende led ganador ...
            {
                PORTBbits.RB2 = 0;                       // Se bloquea el boton de jugador 2
            }
            else if (PORTAbits.RA1 == 1)                 // Si jugador 2 enciende led ganador ...
            {
                PORTBbits.RB1 = 0;                       // Se bloquea el boton de jugador 1
            }
            else                                         // Si nadie ha ganado, el juego continua
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

void semaforo(void) 
{
    //*************************************************************************
    // Enciende y apaga el led rojo y amarillo del semaforo con un delay de 
    // 700ms para que cada color este encendido ese tiempo, luego regresa 
    // al loop principal
    //*************************************************************************
    
    LED_rojo = 1;
    __delay_ms(700);
    LED_rojo = 0;
    LED_amarillo = 1;
    __delay_ms(700);
    LED_amarillo = 0;
    return;      
}

void verde(void) 
{    
    //*************************************************************************
    // Enciende el led verde y apaga el rojo y amarillo durante el juego
    //*************************************************************************
    
    LED_verde = 1;
    LED_rojo = 0;
    LED_amarillo = 0;
}

void jugador_1(void) 
{
    //*************************************************************************
    // Incremento de contador de decadas al presionar un push button
    // el cual esta en pull up y tiene anti-rebote
    //*************************************************************************
    
    if (PORTAbits.RA2 == 1)                     // Verifica que el led ya este en verde
    {
        if (PORTBbits.RB1 == 0)                 // Verifica que el boton este presionado 
        {
            pressed_ok = pressed_ok + 1;        // Se incrementa contador que verifica que el boton este presionado con rango de seguridad 
            released_ok = 0;                    // Variable de boton libre se reduce a cero porque boton se esta presionando
            if (pressed_ok > 500)               // Si el boton esta seguramente presionado
            {
                if (presionado == 0)            // Verifica que el boton esta en posicion presionado
                {    
                    PORTC = pow(2,i);           // Incrementa el contador de decada del puerto
                    i = i + 1;                  // Incrementa el entero i para calcular el siguiente valor
                    presionado = 1;             // Coloca el boton como ya presionado para no volver a repetir este ciclo
                    if (i == 8)                 // Si i no llega hasta 8...
                    {
                        PORTAbits.RA0 = 1;      // El led del ganador no se enciende
                    }
                }
                pressed_ok = 0;                 // Se reduce contador de boton de seguridad presionado para siguiente ciclo
            }
        }
        else                                    // Si el boton no esta presionado
        {
            released_ok = released_ok + 1;      // Se incrementa contador que verifica que el boton este libre con rango de seguridad 
            pressed_ok = 0;                     // Contador de boton presionado se reduce a cero porque boton esta libre
            if (released_ok > 500)              // Verifica que el boton este libre ...
            {
                presionado = 0;                 // Coloca el boton como libre para siguiente ciclo
                released_ok = 0;                // Variable de boton libre se reduce a cero para siguiente ciclo
            }
        }
    }
}

void jugador_2(void) 
{
    //*************************************************************************
    // Incremento de contador de decadas al presionar un push button
    // el cual esta en pull up y tiene anti-rebote
    //*************************************************************************
    if (PORTAbits.RA2 == 1)                      // Verifica que el led ya este en verde
    {
        if (PORTBbits.RB2 == 0)                  // Verifica que el boton este presionado 
        {
            pressed_ok2 = pressed_ok2 + 1;       // Se incrementa variable que verifica que el boton este presionado con rango de seguridad 
            released_ok2 = 0;                    // Variable de boton libre se reduce a cero porque boton se esta presionando
            if (pressed_ok2 > 500)               // Si el boton esta seguramente presionado
            {
                if (presionado2 == 0)            // Verifica que el boton esta en posicion presionado
                {    
                    PORTD = pow(2,i2);           // Incrementa el contaodr de decada del puerto
                    i2 = i2 + 1;                 // Incrementa el entero i para calcular el siguiente valor
                    presionado2 = 1;             // Coloca el boton como ya presionado para no volver a repetir este ciclo
                    if (i2 == 8)                 // Si i no llega hasta 8...
                    {
                        PORTAbits.RA1 = 1;       // El led del ganador no se enciende
                    }
                }
                pressed_ok2 = 0;                 // Se reduce contador de boton de seguridad presionado para siguiente ciclo
            }
        }
        else                                     // Si el boton no esta presionado
        {
            released_ok2 = released_ok2 + 1;     // Se incrementa variable que verifica que el boton este libre con rango de seguridad 
            pressed_ok2 = 0;                     // Variale de boton presionado se reduce a cero porque boton esta libre
            if (released_ok2 > 500)              // Verifica que el boton este libre ...
            {
                presionado2 = 0;                 // Coloca el boton como libre para siguiente ciclo
                released_ok2 = 0;                // Variable de boton libre se reduce a cero para siguiente ciclo
            }
        }
    }
}
                  
 

