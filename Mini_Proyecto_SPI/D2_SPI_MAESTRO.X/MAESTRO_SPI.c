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
#include "LCD_SPI.h"          // Libreria Personalizada LCD
#include "USART_SPI.h"              // Libreria Personalizada comunicacion serial
#include "SPI_SPI.h"

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

uint8_t decenay;
uint8_t voltajey;               // Conversion a LCD3
uint8_t unidady;
uint8_t x1y;
uint8_t y1y;
uint8_t x2y;
uint8_t y2y;

uint8_t leer;                   // Comunicacion Serial
uint8_t contador;

uint8_t w1;                     // Conversion Terminal Virtual
uint8_t w2;
uint8_t a;
uint8_t velocidad1 = 0;         
uint8_t velocidad2 = 0;   
uint8_t velocidad3 = 0; 

uint8_t dato_pot = 0;
uint8_t dato_push = 0;
uint8_t dato_push1 = 0;
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
{                               // GIE = 0
    if (INTCONbits.TMR0IF == 1)  // Si hay desboradmiento de TIMER0 la bandera se levanta y se revisa
    {
        INTCONbits.TMR0IF = 0;   // Se apaga la bandera manualmente
        TMR0 = 100;                
    }
}                                      // GIE = 1

//============================================================================*/
// CICLO PRINCIPAL
//============================================================================*/

void main(void) 
{
    setup();                            // Funciones de Configuracion
    osc_config();
    interrup_config();
    tmr0_config ();
    Lcd_Init();
    USART_config();  
    SPI_config ();
    while (1)                           // Loop principal
    {
               
        PORTCbits.RC0 = 0;
        __delay_ms(1);
        
        SPI_Enviar (dato_pot);        
        dato_pot  = SPI_Recibir();
        
        __delay_ms(1);        
        PORTCbits.RC0 = 1;
        
        //----------------------------------------------
        
        PORTCbits.RC1 = 0;
        __delay_ms(1);
        
        SPI_Enviar (dato_push);        
        dato_push  = SPI_Recibir();
        
        __delay_ms(1);        
        PORTCbits.RC1 = 1;
        
        //----------------------------------------------
        
        PORTCbits.RC2 = 0;
        __delay_ms(1);
        
 
        SPI_Enviar (dato_semaforo);        
        dato_semaforo  = SPI_Recibir();
               
        __delay_ms(1);        
        PORTCbits.RC2 = 1;
        
        
      
    
                
        lcd ();                         // LCD
        velocidad1 = velocidad1 + 1;    // Velocidad terminal
        velocidad2 = velocidad2 + 1;
        velocidad3 = velocidad3 + 1;
        
        
    }
}

//============================================================================*/
// CONFIGURACION
//============================================================================*/

void setup(void) 
{
    ANSEL = 0;
    TRISA = 0;                // Puerto A como entrada 
    PORTA = 0;                // Puerto A entrada apagado
    ANSELH = 0;               // Puerto B digital
    TRISB = 0;                // Puerto B salida
    PORTB = 0;                // Puerto B RB0 y RB1 entrada igual a 0
    TRISC = 0;                // Puerto C salida leds
    PORTC = 0;                // Puerto C salida leds apagados
    TRISCbits.TRISC6 = 0;     // TX salida
    TRISCbits.TRISC4 = 1;     // Dato de esclavo
    PORTCbits.RC0 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC2 = 1;
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
    OPTION_REGbits.nRBPU = 0; // PORTB pull-ups habilitados
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

void  USART_config(void)      
{
    USART_lib_config();       // USART
}

void SPI_config (void)
{
    SPI_Maestro_Init (0, 2);
}

//============================================================================*/
// FUNCIONES
//============================================================================*/

void  lcd (void)
{
    Lcd_Set_Cursor(0,1);                     // Cursor
    Lcd_Write_String("ADC:  BTN: TEMP:");   // Titulos
    Lcd_Set_Cursor(2,0);                     // Cursor
    Conversion1 (); 
    Lcd_Set_Cursor(2,7);                // Cursor en columna 14// Converion a Char Pot 1
    Conversion2 ();                          // Converion a Char Pot 2
    Lcd_Set_Cursor(2,12);                    // Cursor
    Conversion3 ();                          // Converion a Char Pot 3
    
}

void Conversion1 ()              
{
    voltaje = dato_pot * 2;                  
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
    if (dato_push < 10)                       // Menor a 10?
    {
        Lcd_Write_Char(48);
        Lcd_Write_Char(dato_push+ 48);       // Se escribe
        virtual_display2();               // Valores a terminal virtual
    }
    else if (dato_push >= 10)                 // Mayor a 10?
    {
        w1 = dato_push/10;                    // Decena
        w2 = dato_push % 10;                  // Unidad
        Lcd_Write_Char(w1+48);               // Se escriben valores
        Lcd_Write_Char(w2+48);
        virtual_display2();               // Valores a terminal virtual
    }
}

void Conversion3 ()              
{
    voltajey = dato_semaforo * 2;
    decenay = voltajey / 10;
    unidady = voltajey % 10;         // Unidad sobre 5V
    Lcd_Write_Char(decenay+48);       // Decena 
    Lcd_Write_Char(unidady+48);       // Unidad
    Lcd_Write_Char(223);              // grados
    Lcd_Write_Char(67);               // C
    virtual_display3();               // Valores a terminal virtual
}

void virtual_display1 (void)
{
    if (velocidad1 > 15)              // Velocidad > 15?
    {
        escribir_char (65);           // S
        escribir_char (68);           // 1
        escribir_char (67);           // 1
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
    if (velocidad2 > 15)              // Velocidad > 15?       
    {
        if (dato_push  < 10)                // Contador < 10?
        {
            escribir_char (67);            // C
            escribir_char (79);            // O
            escribir_char (78);            // N
            escribir_char (84);            // T
            escribir_char (58);            // :
            escribir_char (48);            // 0
            escribir_char (dato_push+48);   // Contador en ASCII   
            escribir_char (32);           // espacio en blanco
            escribir_char (32);           // espacio en blanco 
            escribir_char (32);           // espacio en blanco
            escribir_char (32);           // espacio en blanco   
            
            velocidad2 = 0;                
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
            escribir_char (32);           // espacio en blanco
            escribir_char (32);           // espacio en blanco  
            escribir_char (32);           // espacio en blanco
            escribir_char (32);           // espacio en blanco   
            
            velocidad2 = 0;
        }              // se reinicia Velocidad
    }
}

void virtual_display3 (void)
{
    if (velocidad3 > 15)              // Velocidad > 15?
    {
        escribir_char (84);            // T
        escribir_char (69);            // E
        escribir_char (77);            // M
        escribir_char (80);            // P
        escribir_char (58);            // :
        escribir_char (decenay+48);    // Unidad en ASCII    
        escribir_char (unidady+48);    // Unidad en ASCII
        escribir_char (176);           // Primer numero
        escribir_char (67);            // Segundo numero
        escribir_char ('\r');          // Enter
        velocidad3 = 0;               // se reinicia Velocidad
    }
}

void escribir_char (uint8_t valor)
{
    TXREG = valor;                     // Se envia Byte a TXREG
    while (PIR1bits.TXIF == 0);        // Espera a que se haya enviado dato
}


