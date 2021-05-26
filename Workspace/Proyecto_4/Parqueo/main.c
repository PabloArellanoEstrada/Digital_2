//*****************************************************************************
//
// Universidad del Valle de Guatemala
// Ingenieria Mecatronica
// Digital 2 - Proyecto
// Pablo Rene Arellano Estrada
// 151379
//
//*****************************************************************************

#include <stdint.h>            // variables de ancho definido
#include <stdbool.h>           // true or false
#include "inc/hw_memmap.h"     // mapa de memoria
#include "driverlib/debug.h"
#include "driverlib/gpio.h"    // GPIO - Puertos
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h" // Interrupciones
#include "driverlib/fpu.h"

#define XTAL 16000000

uint8_t luz1;                // verificar luz
uint8_t luz2;
uint8_t luz3;
uint8_t luz4;

uint8_t cont1 = 0;           // contar
uint8_t cont2 = 0;
uint8_t cont3 = 0;
uint8_t cont4 = 0;
uint8_t total = 0;

unsigned char char1 = '0';        // enviar char
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

    UARTEnable(UART1_BASE);
}

void UART0config(void){

    // Enable reloj del UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Colocar GPIO C4 - C5 como pines para UART
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    // Enable los pines para ser perifericos
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Inicio Modulo UART: 115200, 8 bits de datos, 1 stop bit, No Paridad
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTEnable(UART0_BASE);
}



//*****************************************************************************
//
// Main
//
//*****************************************************************************
int main(void)
{
    volatile uint32_t ui32Loop;

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


    // Enable GPIO para el LED (PA3). Pines de salida y digitales
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1 |GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    // Enable GPIO para el puerto A. Pines de entrada
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_5);

    // Configuracion: Puerto, Pines, Corriente, Pull Up de la Tiva
    //GPIOPadConfigSet(GPIO_PORTA_BASE,  GPIO_PIN_4, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTA_BASE,  GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTE_BASE,  GPIO_PIN_2, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTE_BASE,  GPIO_PIN_5, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTA_BASE,  GPIO_PIN_5, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    UART1config();
    //UART0config();

    // Loop
    //
    while(1)
    {


        luz1 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4);
        if (luz1 == 0)
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_3);
            cont1 = 0;
            char1 = '1';
        }
        else
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_2);
            cont1 = 1;
            char1 = '0';

        }





        luz2 = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2);
        if (luz2 == 0)
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_3, GPIO_PIN_3);
            cont2 = 0;
            char2 = '1';

        }
        else
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_3, GPIO_PIN_6);
            cont2 = 1;
            char2 = '0';
        }





        luz3 = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_5);
        if (luz3 == 0)
        {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1 |GPIO_PIN_3, GPIO_PIN_3);
            cont3 = 0;
            char3 = '1';
        }
        else
        {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1 |GPIO_PIN_3, GPIO_PIN_1);
            cont3 = 1;
            char3 = '0';

        }






        luz4 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5);
        if (luz4 == 0)
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_6);
            cont4 = 0;
            char4 = '1';
        }
        else
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_7);
            cont4 = 1;
            char4 = '0';
        }



        total = cont1 + cont2 + cont3 + cont4;
        if (total == 0)
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
/*



*/

        UARTCharPut(UART1_BASE,char1);
        UARTCharPut(UART1_BASE,',');
        UARTCharPut(UART1_BASE,char2);
        UARTCharPut(UART1_BASE,',');
        UARTCharPut(UART1_BASE,char3);
        UARTCharPut(UART1_BASE,',');
        UARTCharPut(UART1_BASE,char4);
        UARTCharPut(UART1_BASE,',');

    }

}





