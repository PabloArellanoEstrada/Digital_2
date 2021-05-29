//*****************************************************************************
//
// Universidad del Valle de Guatemala
// Ingenieria Mecatronica
// Digital 2 - Proyecto
// Pablo Rene Arellano Estrada
// 151379
//
//*****************************************************************************

#include <stdint.h>              // Variables de Ancho Definido
#include <stdbool.h>             // True or False
#include "inc/hw_memmap.h"       // Mapa de Memoria
#include "driverlib/debug.h"
#include "driverlib/gpio.h"      // Puertos
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"    // Dirverlib
#include "driverlib/timer.h"     // Timer
#include "driverlib/systick.h"
#include "driverlib/uart.h"      // UART
#include "driverlib/pin_map.h"   // Pines
#include "driverlib/interrupt.h" // Interrupciones
#include "driverlib/fpu.h"

#define XTAL 16000000            // Frecuencia de Osilacion

uint8_t luz1;                    // Sensores
uint8_t luz2;
uint8_t luz3;
uint8_t luz4;

uint8_t cont1 = 0;               // Contadores Tiva
uint8_t cont2 = 0;
uint8_t cont3 = 0;
uint8_t cont4 = 0;
uint8_t total = 0;

unsigned char char1 = '0';       // UART para ESP32
unsigned char char2 = '0';
unsigned char char3 = '0';
unsigned char char4 = '0';

//*****************************************************************************
//
// Rutina de error por si hay error en la libreria
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

//*****************************************************************************
//
// Configuracion UART
//
//*****************************************************************************

void UART1config(void){

    // Enable reloj del UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    // Colocar GPIO C4 - C5 como pines para UART
    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinConfigure(GPIO_PC5_U1TX);

    // Enable los pines para ser perifericos
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // Inicio Modulo UART: 115200, 8 bits de datos, 1 stop bit, No Paridad
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // Enable del UART1
    UARTEnable(UART1_BASE);
}

//*****************************************************************************
//
// Main
//
//*****************************************************************************
int main(void)
{

    // Reloj del Sistema: Principal | 16 MHz | (400 MHz/2) = 200 MHz |  (200MHz/5) = 40 MHz
    SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ |    SYSCTL_USE_PLL    |   SYSCTL_SYSDIV_5  );

    // Enable del reloj Puerto A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    // Verifica si hay acceso a perifericos
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {}

    // Enable del reloj Puerto E
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    // Verifica si hay acceso a perifericos
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {}

    // Enable del reloj Puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Verifica si hay acceso a perifericos
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {}

    // Enable del reloj Puerto D
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    // Verifica si hay acceso a perifericos
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {}

    // Enable del reloj Puerto B
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    // Verifica si hay acceso a perifericos
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {}

    // Enable del reloj Puerto C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    // Verifica si hay acceso a perifericos
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
    {}

    // Enable GPIO para el LED. Pines de salida y digitales
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_3 );
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_3 );

    // Enable GPIO para el DISPLAY. Pines de salida y digitales
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    // Enable GPIO. Pines de entrada
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_5);

    // Configuracion: Puerto, Pines, Corriente, Pull Up de la Tiva
    GPIOPadConfigSet(GPIO_PORTA_BASE,  GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTE_BASE,  GPIO_PIN_2, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTE_BASE,  GPIO_PIN_5, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTA_BASE,  GPIO_PIN_5, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    // Funcion Cofiguracion UART
    UART1config();

    while(1)
    {

        //******************************************************************************************

        luz1 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4);    // Sensor uno recibe luz?
        if (luz1 == 0)
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_3);
            cont1 = 0;
            char1 = '1';                                    // Led Rojo y contador en 0
        }
        else
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_2);
            cont1 = 1;
            char1 = '0';                                    // Led Verde y contador en 1
        }

        //******************************************************************************************


        luz2 = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2);    // Sensor uno recibe luz?
        if (luz2 == 0)
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_3, GPIO_PIN_3);
            cont2 = 0;
            char2 = '1';                                    // Led Rojo y contador en 0
        }
        else
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_3, GPIO_PIN_6);
            cont2 = 1;
            char2 = '0';                                    // Led Verde y contador en 1
        }

        //******************************************************************************************



        luz3 = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_5);     // Sensor uno recibe luz?
        if (luz3 == 0)
        {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1 |GPIO_PIN_3, GPIO_PIN_3);
            cont3 = 0;
            char3 = '1';                                     // Led Rojo y contador en 0
        }
        else
        {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1 |GPIO_PIN_3, GPIO_PIN_1);
            cont3 = 1;
            char3 = '0';                                     // Led Verde y contador en 1
        }

        //******************************************************************************************

        luz4 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5);     // Sensor uno recibe luz?
        if (luz4 == 0)
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_6);
            cont4 = 0;
            char4 = '1';                                     // Led Rojo y contador en 0
        }
        else
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_7);
            cont4 = 1;
            char4 = '0';                                     // Led Verde y contador en 1
        }

        //**************************************************************************************************************************************

        total = cont1 + cont2 + cont3 + cont4;
        if (total == 0)                                     // Display dependiendo del numero
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
                                          GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
        }
        else if (total == 1)
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
                                          GPIO_PIN_2 | GPIO_PIN_3);
        }
        else if (total == 2)
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
                                          GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7 | GPIO_PIN_5 | GPIO_PIN_4);
        }
        else if (total == 3)
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
                                          GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_7);
        }
        else if (total == 4)
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
                                          GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3);
        }

        //**************************************************************************************************************************************

        UARTCharPut(UART1_BASE,char1);    // 0 o 1        // La cadena enviada es 0,0,0,0,
        UARTCharPut(UART1_BASE,',');      // ,
        UARTCharPut(UART1_BASE,char2);    // 0 o 1
        UARTCharPut(UART1_BASE,',');      // ,
        UARTCharPut(UART1_BASE,char3);    // 0 o 1
        UARTCharPut(UART1_BASE,',');      // ,
        UARTCharPut(UART1_BASE,char4);    // 0 o 1
        UARTCharPut(UART1_BASE,',');      // ,

    }
}
