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

#define	RS PORTCbits.RC0
#define	RW PORTCbits.RC1
#define	E  PORTCbits.RC2

#define D0 PORTDbits.RD0
#define D1 PORTDbits.RD1
#define D2 PORTDbits.RD2
#define D3 PORTDbits.RD3
#define D4 PORTDbits.RD4
#define D5 PORTDbits.RD5
#define D6 PORTDbits.RD6
#define D7 PORTDbits.RD7

#include <xc.h>                 // 
#include <stdint.h>             // Variables de ancho definido
#include <stdio.h>              // Variables
#include "ADC_lib.h"            // Libreria Personalizada ADC
#include "LCD_8bits.h"     
#include "USART.h"


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

uint16_t i = 0;                  // Variables Configuracion ADC
uint8_t adc_value = 0;
uint8_t voltaje;

//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void setup(void);
void osc_config (void);
void interrup_config (void);
void tmr0_config(void);
void adc_config (void);
void USART_config(void); 
void lcd (void);
void Conversion (void); 

//============================================================================*/
// INTERRUPCIONES
//============================================================================*/

void __interrupt() ISR(void) 
{
                                 // La interrupcion global GIE inicia automaticamente con GIE = 0
    if (INTCONbits.TMR0IF == 1)  // Si hay desboradmiento de TIMER0 la bandera se levanta y se revisa
    {
        INTCONbits.TMR0IF = 0;   // Se apaga la bandera manualmente
        TMR0 = 231; 
        
        
    }                            // La interrupcion global GIE finaliza automaticamente con GIE = 1 para la siguiente
}

//============================================================================*/
// CICLO PRINCIPAL
//============================================================================*/

void main(void) 
{
    setup();                            // Funciones de Configuracion
    osc_config();
    interrup_config();
    Lcd_Init();
    tmr0_config();
    adc_config ();
    USART_config();
    while (1)                           // LOOP PRINCIPAL **********************
    {
        lcd ();
        ADCON0bits.GO_DONE = 1;         // Se inicia el GO_DONE para iniciar conversion
        __delay_ms(10);                 // Se da tiempo para el Acquisition Time Example
        if (ADCON0bits.GO_DONE == 0)    // Si ya termino la conversion
        {
            ADCON0bits.GO_DONE = 1;     // Se inicia el GO_DONE para iniciar nuevamente
            adc_value = ADRESH;         // Se Coloca el valor del registro de la conversion en una variable     
            if (PIR1bits.TXIF == 1)
            {
                TXREG  = ADRESH;
            }
        }
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
    TRISB = 0;                // 
    PORTB = 0;                // Puerto B RB0 y RB1 entrada igual a 0
    TRISC = 0;                // Puerto C salida leds
    PORTC = 0;                // Puerto C salida leds apagados
    TRISD = 0;                // Puerto D salida display
    PORTD = 0;                // Puerto D salida apagados
    TRISE = 0;                // Puerto E salida transistores y alarma
    PORTE = 0;                // Puerto E salida apagado
}

void interrup_config (void) 
{
    INTCONbits.GIE = 1;       // Interrupciones globales habilitadas
    INTCONbits.PEIE = 0;      // Interrupciones periferias deshabilidatas
    INTCONbits.T0IE = 1;      // Interrupcion del Timer0 habilitada
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
    TMR0 = 231;                // Valor del TIMER0 para un delay de 0.246 seg.
}

//============================================================================*/
// FUNCIONES CON LIBRERIA
//============================================================================*/

void adc_config (void)
{
    initADC (0);              // Configuracion de ADC en libreria
}

void  USART_config(void)                       // Valor del pic a compu de dos potensiometros
{
    USART_lib_config();
}




//============================================================================*/
// FUNCIONES
//============================================================================*/

void  lcd (void)                       // Valor del pic a compu de dos potensiometros
{
    unsigned int a;
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("S1:   S2:   S3: ");
    Lcd_Set_Cursor(2,0);
    Conversion (); 
    Lcd_Set_Cursor(2,6);
    Conversion (); 
    
    
}

void Conversion ()              // Conversion de Binario a Voltaje
{
    voltaje = ADRESH * 2;
    char unidad = voltaje / 100;
    
    char x1 = voltaje % 100;
    char x2 = x1 / 10;
    
    char y1 = x1 % 10;
    char y2 = y1 / 1;
    
    Lcd_Write_Char(unidad+48); 
    Lcd_Write_Char(46);
    Lcd_Write_Char(x2+48);
    Lcd_Write_Char(y2+48);  
    Lcd_Write_Char(86);
}



// void USART ()                       // Valor del pic a compu de dos potensiometros
//{
    // ADCONL = 0;
    // ADCON0 = 0;
    // ADCON0bits.GO_DONE = 1;         // Se inicia el GO_DONE para iniciar conversion
    // __delay_ms(10);                 // Se da tiempo para el Acquisition Time Example
    // if (ADCON0bits.GO_DONE == 0)    // Si ya termino la conversion
    // {
    //      ADCON0bits.GO_DONE = 1;     // Se inicia el GO_DONE para iniciar nuevamente
    //      adc_value_1 = ADRESL;         // Se Coloca el valor del registro de la conversion en una variable
    //      PIR1bits.ADIF = 0;
    //      if (PIR1bits.TXIF = 1)
    //      {
    //           TXREG = adc_value_1;
    //      }
    // } 

    // ADCONL = 0;
    // ADCON0 = 0;
    // ADCON0bits.GO_DONE = 1;         // Se inicia el GO_DONE para iniciar conversion
    // __delay_ms(10);                 // Se da tiempo para el Acquisition Time Example
    // if (ADCON0bits.GO_DONE == 0)    // Si ya termino la conversion
    // {
    //      ADCON0bits.GO_DONE = 1;     // Se inicia el GO_DONE para iniciar nuevamente
    //      adc_value_2 = ADRESL;         // Se Coloca el valor del registro de la conversion en una variable
    //      PIR1bits.ADIF = 0;
    //      if (PIR1bits.TXIF = 1)
    //      {
    //           TXREG = adc_value_2;
    //      }
    // } 
//}




// void contador ()                    // Valor de Compu a LCD
//{
    // if (valor "+")
    // {
    //      contador = contador + 1;
    // }

    // if (valor "-")
    // {
    //      contador = contador - 1;
    // }
//}