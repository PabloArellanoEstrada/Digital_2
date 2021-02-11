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
#define _XTAL_FREQ 4000000


//============================================================================*/
// VARIABLES
//============================================================================*/

uint16_t i = 0;                  // Variables Configuracion ADC
uint8_t adc_value1 = 0; 
uint8_t adc_value2 = 0; 

uint8_t voltaje;
uint8_t unidad;
uint8_t x1;
uint8_t y1;
uint8_t x2;
uint8_t y2;

uint8_t voltajez;
uint8_t unidadz;
uint8_t x1z;
uint8_t y1z;
uint8_t x2z;
uint8_t y2z;


uint8_t leer;  
uint8_t contador;

uint8_t compu_valor1;
uint8_t compu_valor2;
uint8_t aux1;
uint8_t aux2;


uint16_t pressed_ok2 = 0;       
uint16_t pressed_ok1 = 0;   


//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void setup(void);
void osc_config (void);
void interrup_config (void);
void adc_config (void);
void USART_config(void); 
void lcd (void);
void Conversion1 (void); 
void Conversion2 (void); 
void escribir_char (uint8_t valor);
char leer_char(void);
void contador_lcd(void) ;
void virtual_display1 (void);
void virtual_display2 (void);
void adc_conversion1 (void);
void adc_conversion2 (void);
void tmr0_config (void); 

//============================================================================*/
// INTERRUPCIONES
//============================================================================*/

void __interrupt() ISR(void) 
{
    // La interrupcion global GIE inicia automaticamente con GIE = 0
    if (PIR1bits.RCIF == 1)                                                    ///////////////////
    {
        leer = leer_char();
        PIR1bits.RCIF = 0;      
    } 
    
      if (INTCONbits.TMR0IF == 1)  // Si hay desboradmiento de TIMER0 la bandera se levanta y se revisa
    {
        INTCONbits.TMR0IF = 0;   // Se apaga la bandera manualmente
        unidad = unidad;
        x2 = x2;
        y2 = y2;
        
        TMR0 = 10;      
        
      }
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
    Lcd_Init();
    adc_config ();
    USART_config();  
    
    while (1)                           // LOOP PRINCIPAL **********************
    {
        lcd ();
        pressed_ok1 = pressed_ok1 + 1;
        pressed_ok2 = pressed_ok2 + 1;
       
        initADC (0);
        adc_conversion1 ();
        initADC (1);
        adc_conversion2 ();
    }
}

//============================================================================*/
// CONFIGURACION
//============================================================================*/

void setup(void) 
{
    ANSELbits.ANS0 = 1;       // Puerto A analogico
    ANSELbits.ANS1 = 1;       // Puerto A analogico
    TRISA = 1;                // Puerto A como entrada 
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    PORTA = 0;                // Puerto A entrada apagado
    ANSELH = 0;               // Puerto B digital
    TRISB = 0;                // 
    PORTB = 0;                // Puerto B RB0 y RB1 entrada igual a 0
    TRISC = 0;                // Puerto C salida leds
    TRISCbits.TRISC6 = 0;     // TX salida
    TRISCbits.TRISC7 = 1;     // RX entrada
    PORTC = 0;                // Puerto C salida leds apagados
    TRISD = 0;                // Puerto D salida display
    PORTD = 0;                // Puerto D salida apagados
    TRISE = 0;                // Puerto E salida transistores y alarma
    PORTE = 0;                // Puerto E salida apagado
}

void interrup_config (void) 
{
    INTCONbits.GIE = 1;       // Interrupciones globales habilitadas
    INTCONbits.PEIE = 1;      // Interrupciones periferias deshabilidatas
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
    TMR0 = 200;                // Valor del TIMER0 para un delay de 0.246 seg.
}

//============================================================================*/
    // CONFIGURACION CON LIBRERIA
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

void adc_conversion1 (void)
{
    ADCON0bits.GO_DONE = 1;         // Se inicia el GO_DONE para iniciar conversion
        __delay_ms(10);                 // Se da tiempo para el Acquisition Time Example
        if (ADCON0bits.GO_DONE == 0)    // Si ya termino la conversion
        {
            ADCON0bits.GO_DONE = 1;     // Se inicia el GO_DONE para iniciar nuevamente
            adc_value1 = ADRESH;         // Se Coloca el valor del registro de la conversion en una variable     
        }
}

void adc_conversion2 (void)
{
    ADCON0bits.GO_DONE = 1;         // Se inicia el GO_DONE para iniciar conversion
        __delay_ms(10);                 // Se da tiempo para el Acquisition Time Example
        if (ADCON0bits.GO_DONE == 0)    // Si ya termino la conversion
        {
            ADCON0bits.GO_DONE = 1;     // Se inicia el GO_DONE para iniciar nuevamente
            adc_value2 = ADRESH;         // Se Coloca el valor del registro de la conversion en una variable     
        }
}

void  lcd (void)
{
    unsigned int a;
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("S1:   S2:   S3: ");
    Lcd_Set_Cursor(2,0);
    Conversion1 (); 
    Lcd_Set_Cursor(2,6);
    Conversion2 (); 
    contador_lcd();
    if (contador < 10)
    {
        Lcd_Set_Cursor(2,14);
        Lcd_Write_Char(contador + 48);
    }
    else if (contador >= 10)
    {
        char w1 = contador/10; 
        char w2 = contador % 10; 
        Lcd_Set_Cursor(2,13);
        Lcd_Write_Char(w1+48);
        Lcd_Write_Char(w2+48);
    }
}

void Conversion1 ()              // Conversion de Binario a Voltaje
{
    voltaje = adc_value1 * 2;
    unidad = voltaje / 100;
    
    x1 = voltaje % 100;
    x2 = x1 / 10;
    
    y1 = x1 % 10;
    y2 = y1 / 1;
    
    Lcd_Write_Char(unidad+48); 
    Lcd_Write_Char(46);
    Lcd_Write_Char(x2+48);
    Lcd_Write_Char(y2+48);  
    Lcd_Write_Char(86);
    virtual_display1();
}

void Conversion2 ()              // Conversion de Binario a Voltaje
{
    voltajez = adc_value2 * 2;
    unidadz = voltajez / 100;
    
    x1z = voltajez % 100;
    x2z = x1z / 10;
    
    y1z = x1z % 10;
    y2z = y1z / 1;
    
    Lcd_Write_Char(unidadz+48); 
    Lcd_Write_Char(46);
    Lcd_Write_Char(x2z+48);
    Lcd_Write_Char(y2z+48);  
    Lcd_Write_Char(86);
    virtual_display2();
}

void virtual_display1 (void)
{
   
    if (pressed_ok1 > 25)              // Si el boton esta seguramente presionado
        {
            escribir_char (83); 
            escribir_char (49); 
            escribir_char (58);     
            escribir_char (unidad+48);                                                     /////////////
            escribir_char (46);
            escribir_char (x2+48);
            escribir_char (y2+48);
            escribir_char (86);
            escribir_char (32);
            escribir_char (32);
            pressed_ok1 = 0;
        }
}
       
void virtual_display2 (void)
{
    if (pressed_ok2 > 25)              // Si el boton esta seguramente presionado
        {
            escribir_char (83); 
            escribir_char (50); 
            escribir_char (58);     
            escribir_char (unidadz+48);                                                     /////////////
            escribir_char (46);
            escribir_char (x2z+48);
            escribir_char (y2z+48);
            escribir_char (86);
            escribir_char (32);
            escribir_char (32);
            pressed_ok2 = 0;
        }
   
}

void escribir_char (uint8_t valor)
{
    TXREG = valor;
    while (PIR1bits.TXIF == 0);
}

char leer_char(void)
{
    if (RCSTAbits.OERR ==0)
    {
        CREN = 0;
        NOP();
        CREN = 1;
    }
    return (RCREG);
} 

void contador_lcd (void)                   
{
    if (leer == '+')
    {
        contador = contador + 1;
        leer = 0;
    }
    else if (leer == '-')
    {
        contador = contador - 1;
        leer = 0;
    }
}

