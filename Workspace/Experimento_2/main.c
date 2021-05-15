//*****************************************************************************
//
// Universidad del Valle de Guatemala
// Ingenieria Mecatronica
// Digital 2 - Laboaratorio
// Pablo Rene Arellano Estrada
// 151379
//
//*****************************************************************************

#include <stdint.h>              // variables de ancho definido
#include <stdbool.h>             // true or false
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"       // mapa de memoria
#include "driverlib/debug.h"
#include "driverlib/gpio.h"      // GPIO - Puertos
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"    // Libreria importada
#include "driverlib/interrupt.h" // Interrupciones
#include "driverlib/uart.h"      // UART
#include "driverlib/timer.h"     // Timer
#include "driverlib/systick.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#define XTAL 16000000
uint8_t Status = 0;

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
//  3 y 4. Handler de la interrupción del TIMER 0 - Interrupcion a 0.5Hz
//
//******************************************************************************

void Timer0IntHandler(void)
{
    if (Status == 0)
    {
        // 3a. Encendido - lo apago
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
        }

        // 3b. Apagado - lo enciendo
        else
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 10);
        }

        // 3c. Clear the timer interrupt
        TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    }

}

//*****************************************************************************
//
//  6 y 7. Handler de la interrupción del TIMER 0 - Interrupcion a 0.5Hz
//
//******************************************************************************

void UARTIntHandler(void)
{
    uint32_t ui32Status;

    // 7a. Obtener el Status de la interrupcion
    ui32Status = ROM_UARTIntStatus(UART0_BASE, true);

    // 7b. Limpiar a la interrupcion entrada
    ROM_UARTIntClear(UART0_BASE, ui32Status);

    // 7c. Mientras existan caracteres disponibles
    while(ROM_UARTCharsAvail(UART0_BASE))
    {
        // 7d. Leer caracter
        unsigned char letra = UARTCharGetNonBlocking(UART0_BASE);

        // 7e.Escribir caracter leido
        ROM_UARTCharPutNonBlocking(UART0_BASE,letra);

        if (letra == 'r' || letra == 'R')
        {
            // 7f. Rojo Encendido? - lo apago
            if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1))
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
                Status = 0;
            }

            // 7f. Rojo Apagado? - lo enciendo
            else
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_1);
                Status = 1;
            }
        }

        if (letra == 'g' || letra == 'G')
        {
            // 7f. Encendido? - lo apago
            if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3))
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
                Status = 0;
            }

            // 7f. Apagado? - lo enciendo
            else
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_3);
                Status = 1;
            }
        }

        if (letra == 'b' || letra == 'B')
        {
            // 7f. Encendido? - lo apago
            if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
                Status = 0;
            }

            // 7f. Apagado? - lo enciendo
            else
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_2);
                Status = 1;
            }
        }
    }
}

//*****************************************************************************
//
// Enviar cadena de caracteres por UART
//
//*****************************************************************************

void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    // Loop mientras existan mas caracteres para usar
    while(ui32Count--)
    {
        // Escribir el siguiente caracter por el UART
        ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}

//*****************************************************************************
//
// Main
//
//*****************************************************************************

int main(void)
{
    volatile uint32_t ui32Loop;

    // 2. Reloj del Sistema: Principal | 16 MHz | (400 MHz/2) = 200 MHz |  (200MHz/5) = 40 MHz
    SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ |    SYSCTL_USE_PLL    |   SYSCTL_SYSDIV_5  );

    // 2a. Enable del reloj Puerto F (Leds)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // 2b. Verifica si hay acceso a periferico de Puerto B
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {  }

    // 2c. Enable GPIO para el LED (PF3). Pines de salida y digitales
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    // 2d.  Enable del reloj del Timer0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // 2e. Verifica si hay acceso a periferico del Timer
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
    {  }

    // 2f. Timer periodico
    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);

    // 2g. Timer de 40 millones/2 = 20 millones - 1 = 19.99 millones  para que sea de 1 segundo
    TimerLoadSet(TIMER0_BASE, TIMER_A, ((SysCtlClockGet())/2)-1);

    // 2h. Interrupcion Timer0
    IntEnable(INT_TIMER0A);

    // 2i. Se habilita el Timer
    TimerEnable(TIMER0_BASE, TIMER_A);

    // 4a. Se establece que exista la interrupción por Timeout
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // 4b. Se habilitan las interrupciones Globales
    IntMasterEnable();

    // 5a. Enable reloj Puerto A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // 5b. Enable reloj del UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // 5c. Enable los pines para ser perifericos
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // 5d. Inicio Modulo UART: 115200, 8 bits de datos, 1 stop bit, No Paridad
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // 6a. Interrupcion para modulo UART
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    // 6b. Interrupcion para modulo UART
    IntEnable(INT_UART0);

    // Colocar GPIO A0 - A1 como pines para UART
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTSend((uint8_t *)"Ingrese color: ", 19);
    UARTCharPut(UART0_BASE, '\n');

    // Loop
    while(1)
    {

    }
}




