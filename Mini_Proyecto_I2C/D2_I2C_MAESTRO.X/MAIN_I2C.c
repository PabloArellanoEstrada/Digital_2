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
#include <pic16f887.h>
#include "I2C_LIB.h"
#include "I2C_USART.h"          // Libreria comunicacion serial
// #include "MPU6050_res_define.h"

//============================================================================*/
// PALABRA DE CONFIGURACION
//============================================================================*/
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT    // Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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
#define _XTAL_FREQ 8000000

//============================================================================*/
// VARIABLES
//============================================================================*/

uint8_t valor;
uint16_t contador;
uint8_t str;

//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void setup(void);
void USART_config(void); 
void Led(void); 
void escribir_char (uint8_t valor);
void osc_config (void);
void interrup_config (void);
void tmr0_config (void);

void MPU6050_Init();
char leer_char(void);
void USART_SendString();
void MPU_Start_Loc();

//============================================================================*/
// INTERRUPCIONES

//============================================================================*/

void __interrupt() ISR(void)    
{                                    // GIE = 0
    if (INTCONbits.TMR0IF == 1)      // Desboradmiento? La bandera se levanta y se revisa
    {
        INTCONbits.TMR0IF = 0;       // Se apaga la bandera manualmente
        TMR0 = 100;        
        contador = contador + 1;
        Led();
    }                                // GIE = 1
} 

//============================================================================*/
// CICLO PRINCIPAL
//============================================================================*/

void main(void) {
    setup();
    USART_config(); 
    osc_config();
    interrup_config();
    tmr0_config ();
    MPU6050_Init();		/* Initialize Gyro */
    
    char buffer[20];
	int Ax,Ay,Az,T,Gx,Gy,Gz;
	float Xa,Ya,Za,t,Xg,Yg,Zg;
    
    while(1){
        //I2C_Master_Start();            // iniciar
        //I2C_Master_Write(0x50);        // direccion
        //I2C_Master_Write(6);           // PUERTO B I2C
        //I2C_Master_Stop();
        
        escribir_char (48);            // AAAA USART
        __delay_ms(5);
        escribir_char ('\n');  
        
        
        MPU_Start_Loc();
		//Read Gyro values continuously & send to terminal over UART 
		Ax = (((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(0));
		Ay = (((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(0));
		Az = (((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(0));
		T =  (((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(0));
		Gx = (((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(0));
		Gy = (((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(0));
		Gz = (((int)I2C_Master_Read(0)<<8) | (int)I2C_Master_Read(1));
		I2C_Master_Stop();
             
		//Divide raw value by sensitivity scale factor 
		Xa = (float)Ax/16384.0;	
		Ya = (float)Ay/16384.0;
		Za = (float)Az/16384.0;
		Xg = (float)Gx/131.0;
		Yg = (float)Gy/131.0;
		Zg = (float)Gz/131.0;
		t = ((float)T/340.00)+36.53; // Convert temperature in °/c 
                                          
        //Take values in buffer to send all parameters over USART 
                          
		sprintf(buffer," Ax = %.2f g\t",Xa);
		USART_SendString(buffer);
                                     
		sprintf(buffer," Ay = %.2f g\t",Ya);
		USART_SendString(buffer);
		
		sprintf(buffer," Az = %.2f g\t",Za);
		USART_SendString(buffer);
		// 0xF8 Ascii value of degree '°' on serial 
		sprintf(buffer," T = %.2f%cC\t",t,0xF8);
		USART_SendString(buffer);

		sprintf(buffer," Gx = %.2f%c/s\t",Xg,0xF8);
		USART_SendString(buffer);

		sprintf(buffer," Gy = %.2f%c/s\t",Yg,0xF8);
		USART_SendString(buffer);
		
		sprintf(buffer," Gz = %.2f%c/s\r\n",Zg,0xF8);
		USART_SendString(buffer);
        
       
        //I2C_Master_Start();             // iniciar
        //I2C_Master_Write(0x51);         // direccion
        //PORTD = I2C_Master_Read(0);     // te quiero escuchar, leer, mando o no aknoldwege
        //I2C_Master_Stop();              // leer 
        //__delay_ms(5);
        PORTB++; 
    }
    return;
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
    
    TRISD = 0;                // Puerto D salida 
    PORTD = 0;                // Puerto D apagados
    TRISE = 0;                // Puerto E salida
    PORTE = 0;                // Puerto E apagado
    TRISEbits.TRISE0 = 0;     // TX salida
    TRISEbits.TRISE3 = 0;     // TX salida
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C 100 khz
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
    OSCCONbits.IRCF2 = 1;     // Oscilador en 8Mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;     
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

void USART_config(void)      
{
    USART_lib_config();       // USART
}

void escribir_char (uint8_t valor)
{
    TXREG = valor;                     // Se envia Byte a TXREG
    while (PIR1bits.TXIF == 0);        // Espera a que se haya enviado dato
}


void USART_SendString(const char *str)
{
   while(*str!='\0')                /* Transmit data until null */
   {            
        escribir_char(*str);
        str++;
   }
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

void MPU_Start_Loc()

{
	I2C_Start_Wait(0xD0);	/* I2C start with device write address */
	I2C_Master_Write(0x3B);/* Write start location address to read */ 
	I2C_Repeated_Start(0xD1);/* I2C start with device read address */
}

void MPU6050_Init()		//Gyro initialization function 
{
    
	MSdelay(150);		//Power up time >100ms 
	I2C_Start_Wait(0xD0);	//Start with device write address 
	I2C_Master_Write(0x19);	//Write to sample rate register 
	I2C_Master_Write(0x07);	//1KHz sample rate 
	I2C_Master_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Master_Write(0x6B);	//Write to power management register 
	I2C_Master_Write(0x01);	//X axis gyroscope reference frequency 
	I2C_Master_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Master_Write(0x1A);	// Write to Configuration register 
	I2C_Master_Write(0x00);	// Fs = 8KHz 
	I2C_Master_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Master_Write(0x1B);	// Write to Gyro configuration register 
	I2C_Master_Write(0x18);	// Full scale range +/- 2000 degree/C 
	I2C_Master_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Master_Write(0x38);	// Write to interrupt enable register 
	I2C_Master_Write(0x01);
	I2C_Master_Stop();
}


void Led(void)
{
    if (contador > 1000)
    {
    if (PORTEbits.RE0 == 1)
    {
        PORTEbits.RE0 = 0;
        contador  = 0;
    }
    else
    {
        PORTEbits.RE0 = 1;
        contador  = 0;
    }
    }

}