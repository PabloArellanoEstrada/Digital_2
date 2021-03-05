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
#include "ADC_lib.h"            // Libreria Personalizada ADC
#include "LCD_8bits.h"          // Libreria Personalizada LCD
#include "USART.h"              // Libreria Personalizada comunicacion serial
#include <pic16f887.h>

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

uint8_t i = 0;                  // ADC
uint8_t adc_value1 = 0; 
uint8_t adc_value2 = 0; 

uint8_t voltaje;                // Conversion a LCD1
uint8_t unidad;
uint8_t x1;
uint8_t y1;
uint8_t x2;
uint8_t y2;

uint8_t voltajez;               // Conversion a LCD2
uint8_t unidadz;
uint8_t x1z;
uint8_t y1z;
uint8_t x2z;
uint8_t y2z;

uint8_t leer;                   // Comunicacion Serial
uint8_t contador;

uint8_t w1;                     // Conversion Terminal Virtual
uint8_t w2;
uint8_t a;
uint8_t velocidad1 = 0;         
uint8_t velocidad2 = 0;   
uint8_t velocidad3 = 0; 

//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void setup(void);                    // Configuracion inicial
void osc_config (void);
void interrup_config (void);
void adc_config (void);
void USART_config(void); 

void adc_conversion1 (void);         // ADC
void adc_conversion2 (void);

char leer_char(void);                // USART
void escribir_char (uint8_t valor);

void lcd (void);                     // LCD
void Conversion1 (void); 
void Conversion2 (void); 
void contador_lcd(void) ;

void virtual_display1 (void);        // Terminal virtual
void virtual_display2 (void);
void virtual_display3 (void);

//============================================================================*/
// INTERRUPCIONES
//============================================================================*/

void __interrupt() ISR(void)    
{                               // GIE = 0
    if (PIR1bits.RCIF == 1)     // Se puede leer?                                            
    {
        leer = leer_char();     // Se lee dato
        PIR1bits.RCIF = 0;      // Se apaga bandera
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
    Lcd_Init();
    adc_config ();
    USART_config();  
    while (1)                           // Loop principal
    {
        lcd ();                         // LCD
        velocidad1 = velocidad1 + 1;    // Velocidad terminal
        velocidad2 = velocidad2 + 1;
        velocidad3 = velocidad3 + 1;
        initADC (0);                    // Canal 0
        adc_conversion1 ();             // Conversion canal 0
        initADC (1);                    // Canal 1
        adc_conversion2 ();             // Conversion canal 1
    }
}

//============================================================================*/
// CONFIGURACION
//============================================================================*/

void setup(void) 
{
    ANSELbits.ANS0 = 1;       // Bit 0 analogico
    ANSELbits.ANS1 = 1;       // Bit 1 analogico
    TRISA = 1;                // Puerto A como entrada 
    TRISAbits.TRISA0 = 1;     // Bit 0 entrada
    TRISAbits.TRISA1 = 1;     // Bit 1 entrada
    PORTA = 0;                // Puerto A entrada apagado
    ANSELH = 0;               // Puerto B digital
    TRISB = 0;                // Puerto B salida
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
    INTCONbits.PEIE = 1;      // Interrupciones periferias habilidatas
    INTCONbits.T0IE = 0;      // Interrupcion del Timer0 deshabilitada
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

//============================================================================*/
    // CONFIGURACION CON LIBRERIA
//============================================================================*/

void adc_config (void)
{
    initADC (0);              // ADC
}

void  USART_config(void)      
{
    USART_lib_config();       // USART
}

//============================================================================*/
// FUNCIONES
//============================================================================*/

void adc_conversion1 (void)
{
    ADCON0bits.GO_DONE = 1;            // GO_DONE para iniciar conversion
    __delay_ms(10);                    // Se da tiempo para el Acquisition Time Example
    if (ADCON0bits.GO_DONE == 0)       // Si ya termino la conversion
    {
        ADCON0bits.GO_DONE = 1;        // Se inicia el GO_DONE para iniciar nuevamente
        adc_value1 = ADRESH;           // Registro de la conversion en una variable     
    }
}

void adc_conversion2 (void)
{
    ADCON0bits.GO_DONE = 1;            // GO_DONE para iniciar conversion
    __delay_ms(10);                    // Se da tiempo para el Acquisition Time Example
    if (ADCON0bits.GO_DONE == 0)       // Si ya termino la conversion
    {
        ADCON0bits.GO_DONE = 1;        // Se inicia el GO_DONE para iniciar nuevamente
        adc_value2 = ADRESH;           // Registro de la conversion en una variable     
    }
}

void  lcd (void)
{
    Lcd_Set_Cursor(1,1);                     // Cursor
    Lcd_Write_String("S1:   S2:   S3: ");    // Titulos
    Lcd_Set_Cursor(2,0);                     // Cursor
    Conversion1 ();                          // Converion a Char Pot 1
    Lcd_Set_Cursor(2,6);                     // Cursor
    Conversion2 ();                          // Converion a Char Pot 2
    contador_lcd();                          // Contador
    if (contador < 10)                       // Menor a 10?
    {
        Lcd_Set_Cursor(2,14);                // Cursor en columna 14
        Lcd_Write_Char(contador + 48);       // Se escribe
    }
    else if (contador >= 10)                 // Mayor a 10?
    {
        w1 = contador/10;                    // Decena
        w2 = contador % 10;                  // Unidad
        Lcd_Set_Cursor(2,13);                // Cursor en columna 13
        Lcd_Write_Char(w1+48);               // Se escriben valores
        Lcd_Write_Char(w2+48);
    }
}

void Conversion1 ()              
{
    voltaje = adc_value1 * 2;                  
    unidad = voltaje / 100;           // Unidad sobre 5V
    x1 = voltaje % 100;
    x2 = x1 / 10;                     // Primera posicion despues de punto
    y1 = x1 % 10;
    y2 = y1 / 1;                      // Segunda posicion despues de punto
    Lcd_Write_Char(unidad+48);        // Unidad
    Lcd_Write_Char(46);               // .
    Lcd_Write_Char(x2+48);            // Primer numero
    Lcd_Write_Char(y2+48);            // Segundo numero
    Lcd_Write_Char(86);               // V
    virtual_display1();               // Valores a terminal virtual
}

void Conversion2 ()              
{
    voltajez = adc_value2 * 2;
    unidadz = voltajez / 100;         // Unidad sobre 5V
    x1z = voltajez % 100;
    x2z = x1z / 10;                   // Primera posicion despues de punto
    y1z = x1z % 10;
    y2z = y1z / 1;                    // Segunda posicion despues de punto
    Lcd_Write_Char(unidadz+48);       // Unidad
    Lcd_Write_Char(46);               // .
    Lcd_Write_Char(x2z+48);           // Primer numero
    Lcd_Write_Char(y2z+48);           // Segundo numero 
    Lcd_Write_Char(86);               // V
    virtual_display2();               // Valores a terminal virtual
    virtual_display3();               // Valores de contador
}

void virtual_display1 (void)
{
    if (velocidad1 > 15)              // Velocidad > 15?
    {
        escribir_char (83);           // S
        escribir_char (49);           // 1
        escribir_char (58);           // :
        escribir_char (unidad+48);    // Unidad en ASCII                                               
        escribir_char (46);           // .
        escribir_char (x2+48);        // Primer numero
        escribir_char (y2+48);        // Segundo numero
        escribir_char (86);           // V
        escribir_char (32);           // espacio en blanco
        escribir_char (32);           // espacio en blanco    
        velocidad1 = 0;               // se reinicia Velocidad
    }
}
       
void virtual_display2 (void)
{
    if (velocidad2 > 15)              // Velocidad > 15?       
    {
        escribir_char (83);           // Velocidad > 15?
        escribir_char (50);           // 1
        escribir_char (58);           // :
        escribir_char (unidadz+48);   // Unidad en ASCII                                                    
        escribir_char (46);           // .
        escribir_char (x2z+48);       // Primer numero
        escribir_char (y2z+48);       // Segundo numero
        escribir_char (86);           // V
        escribir_char (32);           // espacio en blanco
        escribir_char (32);           // espacio en blanco 
        velocidad2 = 0;               // se reinicia Velocidad
    }
}

void virtual_display3 (void)
{
    if (velocidad3 > 15)                   // Velocidad > 15?
    {
        if (contador  < 10)                // Contador < 10?
        {
            escribir_char (67);            // C
            escribir_char (58);            // :
            escribir_char (48);            // 0
            escribir_char (contador+48);   // Contador en ASCII       
            escribir_char ('\r');          // Enter
            velocidad3 = 0;                
        }
        else
        {
            escribir_char (67);            // C
            escribir_char (58);            // :
            escribir_char (w1+48);         // Decena en ASCII 
            escribir_char (w2+48);         // Unidad en ASCII 
            escribir_char ('\r');          // Enter
            velocidad3 = 0;
        }
    }
}

void contador_lcd (void)                   
{
    if (leer == '+')                  // +?
    {
        contador = contador + 1;      // Se incrementa Contador
        leer = 0;                     // Se iguala a 0 para dejar de contar
    }
    else if (leer == '-')             // -?
    {
        contador = contador - 1;      // Se incrementa Contador
        leer = 0;                     // Se iguala a 0 para dejar de contar
    }
}


void escribir_char (uint8_t valor)
{
    TXREG = valor;                     // Se envia Byte a TXREG
    while (PIR1bits.TXIF == 0);        // Espera a que se haya enviado dato
}

char leer_char(void)
{
    if (RCSTAbits.OERR ==0)            // Hay error?
    {
        CREN = 0;                      // Apagar modulo para apagar error
        NOP();
        CREN = 1;                      // Enciende una vez no haya error
    }
    return (RCREG);                    // Se envia valor a RCREG
} 


