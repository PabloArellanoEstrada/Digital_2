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
#include "LCD_SPI.h"            // Libreria LCD
#include "USART_SPI.h"          // Libreria comunicacion serial
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

uint8_t voltaje;                // Conversion POT
uint8_t unidad;
uint8_t x1;
uint8_t y1;
uint8_t x2;
uint8_t y2;

uint8_t voltajez;               // Conversion BOTONES
uint8_t unidadz;
uint8_t x1z;
uint8_t y1z;
uint8_t x2z;
uint8_t y2z;

uint8_t decenay;                // Conversion SEMAFORO
uint8_t voltajey;               
uint8_t unidady;
uint8_t x1y;
uint8_t y1y;
uint8_t x2y;
uint8_t y2y;

uint8_t w1;                     // Terminal Virtual
uint8_t w2;
uint8_t a;
uint8_t velocidad1 = 0;         
uint8_t velocidad2 = 0;   
uint8_t velocidad3 = 0; 

uint8_t dato_pot = 0;           // SPI
uint8_t dato_push = 0;
uint8_t dato_semaforo = 0;





//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void setup(void);                    // Configuracion inicial
void osc_config (void);
void interrup_config (void);
void tmr0_config (void);
void USART_config(void); 
void SPI_config (void);

char leer_char(void);                // USART
void escribir_char (uint8_t valor);

void lcd (void);                     // LCD
void Conversion1 (void); 
void Conversion2 (void); 
void Conversion3 (void);  

void virtual_display1 (void);        // Terminal virtual
void virtual_display2 (void);
void virtual_display3 (void);

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
    Lcd_Init();
    USART_config();  
    SPI_config ();
    while (1)                           
    {
               
        PORTCbits.RC0 = 0;               // POT select
        __delay_ms(1);
        
        SPI_Enviar (dato_pot);           // Variable para acceder a informacion
        dato_pot  = SPI_Recibir();       // Se recibe info
        
        __delay_ms(1);        
        PORTCbits.RC0 = 1;               // POT unselected
        
        //----------------------------------------------------------------------
        
        PORTCbits.RC1 = 0;               // PUSH select
        __delay_ms(1);
        
        SPI_Enviar (dato_push);          // Variable para acceder a informacion
        dato_push  = SPI_Recibir();      // Se recibe info
        
        __delay_ms(1);        
        PORTCbits.RC1 = 1;               // PUSH unselected
        
        //----------------------------------------------------------------------
        
        PORTCbits.RC2 = 0;               // SEMAFORO select
        __delay_ms(1);
        
        SPI_Enviar (dato_semaforo);      // Variable para acceder a informacion 
        dato_semaforo  = SPI_Recibir();  // Se recibe info
               
        __delay_ms(1);        
        PORTCbits.RC2 = 1;               // SEMAFORO unselected
        
        lcd ();                          // LCD
        velocidad1 = velocidad1 + 1;     // Velocidad terminal
        velocidad2 = velocidad2 + 1;
        velocidad3 = velocidad3 + 1;
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

void  USART_config(void)      
{
    USART_lib_config();       // USART
}

void SPI_config (void)
{
    SPI_Maestro_Init (0, 2);  // Maestro Port_Mode/4 y con CKP = 0 / CKE = 1
}

//============================================================================*/
// FUNCIONES
//============================================================================*/

void  lcd (void)
{
    Lcd_Set_Cursor(0,1);                     // Cursor
    Lcd_Write_String("ADC:  BTN: TEMP:");    // Titulos
    Lcd_Set_Cursor(2,0);                     // Cursor
    Conversion1 ();                          // Potenciometro
    Lcd_Set_Cursor(2,7);                     // Cursor fila dos
    Conversion2 ();                          // Botones
    Lcd_Set_Cursor(2,12);                    // Cursor
    Conversion3 ();                          // Semaforo
    
}

void Conversion1 ()              
{
    
    voltaje = dato_pot;               // PIC Potenciometro
    int pot = voltaje * 4;            // Conversion
    unidad = pot / 100;               // Unidad 
    x1 = pot % 100;
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
{                                          // PIC Botones
    if (dato_push < 10)                    // Menor a 10?
    {
        Lcd_Write_Char(48);                // 0
        Lcd_Write_Char(dato_push+ 48);     // unidad
        virtual_display2();                // Valores a terminal virtual
    }
    else if (dato_push >= 10)              // Mayor a 10?
    {
        w1 = dato_push/10;                   
        w2 = dato_push % 10;                 
        Lcd_Write_Char(w1+48);             // Decena 
        Lcd_Write_Char(w2+48);             // Unidad
        virtual_display2();                // Valores a terminal virtual
    }
}

void Conversion3 ()              
{
    voltajey = dato_semaforo;         // PIC Semaforo
    decenay = voltajey / 10;
    unidady = voltajey % 10;          
    Lcd_Write_Char(decenay+48);       // Decena 
    Lcd_Write_Char(unidady+48);       // Unidad
    Lcd_Write_Char(223);              // grados
    Lcd_Write_Char(67);               // C
    virtual_display3();               // Valores a terminal virtual
}

void virtual_display1 (void)
{
    if (velocidad1 > 7)              // Velocidad > 15?
    {
        escribir_char (65);           // A
        escribir_char (68);           // D
        escribir_char (67);           // C
        escribir_char (58);           // :
        escribir_char (unidad+48);    // Unidad en ASCII                                               
        escribir_char (46);           // .
        escribir_char (x2+48);        // Primer numero
        escribir_char (y2+48);        // Segundo numero
        escribir_char (86);           // V
        escribir_char (32);           // espacio en blanco
        escribir_char (32);           // espacio en blanco  
        escribir_char (32);           // espacio en blanco
        escribir_char (32);           // espacio en blanco   
        velocidad1 = 0;               // se reinicia Velocidad
    }
}
       
void virtual_display2 (void)
{
    if (velocidad2 > 7)                   // Velocidad > 15?       
    {
        if (dato_push  < 10)               // Contador < 10?
        {
            escribir_char (67);            // C
            escribir_char (79);            // O
            escribir_char (78);            // N
            escribir_char (84);            // T
            escribir_char (58);            // :
            escribir_char (48);            // 0
            escribir_char (dato_push+48);  // Contador en ASCII   
            escribir_char (32);            // espacio en blanco
            escribir_char (32);            // espacio en blanco 
            escribir_char (32);            // espacio en blanco
            escribir_char (32);            // espacio en blanco   
            
            velocidad2 = 0;                // se reinicia Velocidad
        }
        else
        {
            escribir_char (67);            // C
            escribir_char (79);            // O
            escribir_char (78);            // N
            escribir_char (84);            // T
            escribir_char (58);            // :
            escribir_char (w1+48);         // Decena en ASCII 
            escribir_char (w2+48);         // Unidad en ASCII 
            escribir_char (32);            // espacio en blanco
            escribir_char (32);            // espacio en blanco  
            escribir_char (32);            // espacio en blanco
            escribir_char (32);            // espacio en blanco   
            
            velocidad2 = 0;                // se reinicia Velocidad
        }              
    }
}

void virtual_display3 (void)
{
    if (velocidad3 > 7)               // Velocidad > 15?
    {
        escribir_char (84);            // T
        escribir_char (69);            // E
        escribir_char (77);            // M
        escribir_char (80);            // P
        escribir_char (58);            // :
        escribir_char (decenay+48);    // Unidad en ASCII    
        escribir_char (unidady+48);    // Unidad en ASCII
        escribir_char (176);           // grados
        escribir_char (67);            // C
        escribir_char ('\r');          // Enter
        velocidad3 = 0;                // se reinicia Velocidad
    }
}

void escribir_char (uint8_t valor)
{
    TXREG = valor;                     // Se envia Byte a TXREG
    while (PIR1bits.TXIF == 0);        // Espera a que se haya enviado dato
}


