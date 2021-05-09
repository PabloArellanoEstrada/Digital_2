//*****************************************************************************
//
// Universidad del Valle de Guatemala
// Ingenieria Mecatronica
// Digital 2 - Laboaratorio
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

uint8_t Push;
int pressed_ok = 0;                // Jugador 1 debouncing
int released_ok = 0;
int presionado = 0;
int i = 0;

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
// Main
//
//*****************************************************************************
int main(void)
{
    volatile uint32_t ui32Loop;

    // Reloj del Sistema: Principal | 16 MHz | (400 MHz/2) = 200 MHz |  (200MHz/5) = 40 MHz
    SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ |    SYSCTL_USE_PLL    |   SYSCTL_SYSDIV_5  );

    // Enable Puerto F para Leds
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Verifica si hay acceso a perifericos
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    // Enable GPIO para el LED (PF3). Pines de salida y digitales
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    // Desbloquear PF0 SW2 y SW1
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R = 0x0f;

    // Enable GPIO para el puerto F. Pines de entrada
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);

    // Configuracion: Puerto, Pines, Corriente, Pull Up de la Tiva
    GPIOPadConfigSet(GPIO_PORTF_BASE,  GPIO_PIN_0|GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Loop
    //
    while(1)
    {
        Push = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
        if (Push == 17)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
        }

        else if ((Push == 1) || (Push == 16))
        {
            // Encender Led: Puerto, Pines, Valor  para       VERDE
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_3);

            // Delay de 4*(40MHz/3) = 4 segundos
            SysCtlDelay (4*40000000/3);

            // Encender y apagar por un segundo
            for(ui32Loop = 0; ui32Loop < 3; ui32Loop++)
            {

                // Encender Led: Puerto, Pines, Valor        NINGUNO
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);

                // Delay de (40MHZ/3) = 1 segundo
                SysCtlDelay (40000000/3);

                // Encender Led: Puerto, Pines, Valor         VERDE
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_3);

                // Delay de (40MHZ/3) = 1 segundo
                SysCtlDelay (40000000/3);
            }

            // Encender Led: Puerto, Pines, Valor           AMARILLO
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 10);

            // Delay de 2*40MHz/3) = 2 segundos
            SysCtlDelay (2*40000000/3);

            // Encender Led: Puerto, Pines, Valor             ROJO
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_1);

            // Delay de (3*10,000,000) = 3 segundos
            for(ui32Loop = 0; ui32Loop < 10000000; ui32Loop++)
            {
            }
        }
    }
}

//*****************************************************************************
//
// Boton
//
//*****************************************************************************

void boton (void)
{
    boton();
    if (presionado == 1)                   // Boton presionado?
    {
       pressed_ok = pressed_ok + 1;        // Contador presionado con rango de seguridad
       released_ok = 0;                    // Contador suelto en cero
       if (pressed_ok > 500)               // Desborde contador?
       {
           if (presionado == 0)            // Estuvo suelto antes?
           {
               i = i + 1;                  // Incrementa contador
               presionado = 1;             // Se presiono

           }
           pressed_ok = 0;                 // Reinicia contador
        }
    }
    else if (presionado == 17)             // No presinado?
    {
       released_ok = released_ok + 1;      // Contador suelto con rango de seguridad
       pressed_ok = 0;                     // Contador presioando en cero
       if (released_ok > 500)              // Desborde contador?
       {
          presionado = 17;                 // Se solto
          released_ok = 0;                 // Reinicia contador
       }
    }
}
