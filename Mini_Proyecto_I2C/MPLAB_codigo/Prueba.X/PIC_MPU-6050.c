/* 
 * Project: D2_I2C_MAESTRO
 * File:    MAESTRO_SPI.c
 * Author:  Pablo Rene Arellano Estrada
 * Carnet:  151379
 * Created: Match, 2021.
 * Source: KHALED MAGDY 
 * Link:  https://deepbluembedded.com/mpu6050-with-microchip-pic-accelerometer-gyroscope-interfacing-with-pic/
 * Link: http://www.electronicwings.com
 */

//============================================================================*/
// LIBRERIAS
//============================================================================*/
 
#include <xc.h>                       // XC8 libreria
#include <stdint.h>                   // Variables de ancho definido
#include <stdio.h>                    // Tipos de variables, macros, entradas y salidas
#include "USART_Header_File.h"        // USART
#include "I2C_Master_File.h"          // I2C
#include "MPU6050_res_define.h"       // Registros del MPU9250

//============================================================================*/
// PALABRA DE CONFIGURACION
//============================================================================*/

#include "Configuration_header_file.h" // Libreria

//============================================================================*/
// PROTOTIPO DE FUNCIONES
//============================================================================*/

void MPU6050_Init();                // MPU9250
void MPU_Start_Loc();

void setup(void);                   // Configuracion               
void osc_config (void);
void interrup_config (void);
void tmr0_config (void);

//============================================================================*/
// VARIABLES
//============================================================================*/

uint8_t leer;

//============================================================================*/
// INTERRUPCIONES

//============================================================================*/

void __interrupt() ISR(void)    
{                                   
    if (INTCONbits.TMR0IF == 1)      // Desboradmiento? La bandera se levanta y se revisa
    {
        INTCONbits.TMR0IF = 0;       // Se apaga la bandera manualmente
        TMR0 = 100;      
        leer = USART_RxChar();       // Se lee variable en el Rx
        
            
        if (PORTDbits.RD2 == 1)      // Si boton en Adafruit esta en alto
        {
            PORTAbits.RA1 = 1;       // Se enciende Led1
        }
        else
        {
            PORTAbits.RA1 = 0;       // Se apaga Led1
        }
        
         if (PORTDbits.RD3 == 1)     // Si boton en Adafruit esta en alto 
        {
            PORTAbits.RA2 = 1;       // Se enciende Led1
        }
        else
        {
            PORTAbits.RA2 = 0;       // Se apaga Led1
        }
    }                               
} 

//============================================================================*/
// CICLO PRINCIPAL
//============================================================================*/

void main()

{
    setup();
    osc_config ();
    interrup_config ();
    tmr0_config ();
    char buffer [40];           // Cadena de bytes
	int Ax,Ay,Az,T,Gx,Gy,Gz;    // Variables a leer del MPU 
    I2C_Init();		            // Inicia I2C para 100kHz
	MPU6050_Init();		        // Inicia MPU
	USART_Init(9600);	        // Inicia UART
	while(1)
	{
        MPU_Start_Loc();								   // Funcion donde esta la direccion
                                                           // ADC 8 bits + ADC 8 bits = 16 bits
        Ax = (((int)I2C_Read(0)<<8) | (int)I2C_Read(0));   // Aceleracion x  
		Ay = (((int)I2C_Read(0)<<8) | (int)I2C_Read(0));   // Aceleracion y 
		Az = (((int)I2C_Read(0)<<8) | (int)I2C_Read(0));   // Aceleracion z 
		T =  (((int)I2C_Read(0)<<8) | (int)I2C_Read(0));   // Temp
		Gx = (((int)I2C_Read(0)<<8) | (int)I2C_Read(0));   // Giroscopio x 
		Gy = (((int)I2C_Read(0)<<8) | (int)I2C_Read(0));   // Giroscopio x 
		Gz = (((int)I2C_Read(0)<<8) | (int)I2C_Read(1));   // Giroscopio x 
        
		I2C_Stop();                                        // Se acaba lectura del sensor
                   
        int Ax1 = Ax/4;                       // Conversion
        int Ay1 = Ay/4;
        int Az1 = Az/4;
        
        sprintf(buffer,"%d ,\r\n",Ax1);       // Entero con coma        
        USART_SendString(buffer);             // Se envia dato
        
        sprintf(buffer,"%d ,\r\n",Ay1);       // Entero con coma  
        USART_SendString(buffer);             // Se envia dato
         
        sprintf(buffer,"%d ,\r\n",Az1);       // Entero con coma  
        USART_SendString(buffer);             // Se envia dato
                        
        /*        
		///////////////////////////////////////////////////////////////////////  Ax
        
        char decimal = Ax1 /1000;
        char residuo0 = Ax1 % 1000;
        char entero = residuo0 / 100;
        char residuo1 = residuo0 % 100;
        char unidad = residuo1 / 10;
        char residuo2 = residuo1 % 10;
        char decena = residuo2 / 1;
         
        USART_TxChar(65);
        USART_TxChar(120);
        USART_TxChar(61);
        USART_TxChar(32);
        USART_TxChar(decimal+48);
        USART_TxChar(entero+48);
        USART_TxChar(46);
        USART_TxChar(unidad+48);
        USART_TxChar(decena+48);
        USART_TxChar(32);
        USART_TxChar(32);
       */
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
    
    TRISAbits.TRISA1 = 0;     // Led1
    TRISAbits.TRISA2 = 0;     // Led2
    
    TRISCbits.TRISC6 = 0;     // TX salida
    TRISCbits.TRISC7 = 1;     // RX entrada  
    
    TRISDbits.TRISD2 = 1;     // Boton 1 Adafruit entrada
    TRISDbits.TRISD3 = 1;     // Boton 2 Adafruit entrada
    
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

//============================================================================*/
// FUNCIONES
//============================================================================*/


void MPU6050_Init()									
{
	MSdelay(150);		      // Se enciende en en t > 10ms
    
	I2C_Start_Wait(0xD0);	  // Empezar 
	I2C_Write(SMPLRT_DIV);	  // Direccion del registro de ejemplo de frecuencia
	I2C_Write(0x07);	      // Escribir 1kHz - 0b00000111 a esa direccion
	I2C_Stop();               // Se termina transmision

	I2C_Start_Wait(0xD0);     // Empezar 
	I2C_Write(PWR_MGMT_1);	  // Direccion registro de administrador de energia
	I2C_Write(0x01);	      // Frecuenia del SELF_TEST_X_GYRO a esa direccion
	I2C_Stop();               // Se termina transmision

	I2C_Start_Wait(0xD0);     // Empezar 
	I2C_Write(CONFIG);	      // Direccion registro de configuracion
	I2C_Write(0x00);	      // Frecuencia de 8kHz a esa direccion
	I2C_Stop();               // Se termina transmision

	I2C_Start_Wait(0xD0);     // Empezar 
	I2C_Write(GYRO_CONFIG);	  // Direccion registro de configuracion del Giroscopio
	I2C_Write(0x18);	      // Fijar escala del Giroscopio en esa direccion
	I2C_Stop();               // Se termina transmision

	I2C_Start_Wait(0xD0);     // Empezar 
	I2C_Write(INT_ENABLE);	  // Direccion registro de la interrupcion
	I2C_Write(0x01);          // Frecuenia del SELF_TEST_X_GYRO en esa direccion
	I2C_Stop();               // Se termina transmision
}

void MPU_Start_Loc()
{
  I2C_Start_Wait(0xD0);       // Empezar
  I2C_Write(57);              // Direccion donde empieza los datos a leer
  I2C_Stop();
  I2C_Repeated_Start(0xD1);   // Empezar otra vez
  I2C_Read(0);                // Se leen valores dummies
}