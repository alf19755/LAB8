// Parte 1
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"

uint32_t ui32Period;  // Variable para almacenar el periodo del temporizador

void delay(uint32_t ui32Ms);

// Parte 4 - Manejador de interrupci�n del temporizador
void Timer0IntHandler(void) {
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // Limpiar la bandera de interrupci�n del temporizador
    char receivedChar = UARTCharGet(UART0_BASE);  // Obtener un car�cter del m�dulo UART

    // Alternar el estado del LED seg�n el car�cter recibido
    if (receivedChar == 'r') {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1));
    } else if (receivedChar == 'g') {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3));
    } else if (receivedChar == 'b') {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2));
    }
}

// Parte 6 - Manejador de interrupci�n del m�dulo UART
void UARTIntHandler(void) {
    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_RT);  // Limpiar la bandera de interrupci�n del UART
    char receivedChar = UARTCharGet(UART0_BASE);  // Obtener un car�cter del m�dulo UART

    // Alternar el estado del LED seg�n el car�cter recibido
    if (receivedChar == 'r') {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1));
    } else if (receivedChar == 'g') {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3));
    } else if (receivedChar == 'b') {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2));
    }
}

// Parte 2, 3, 5, 7, 8 - Funci�n principal
int main(void) {
    // Configuraci�n del sistema
    SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_320, 40000000);  // Configurar el reloj del sistema
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  // Habilitar el puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  // Habilitar el temporizador 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);  // Habilitar el m�dulo UART 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);  // Habilitar el puerto A para el UART

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);  // Configurar los pines como salidas

    // Configuraci�n del temporizador
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);  // Configurar el temporizador 0 como peri�dico
    ui32Period = SysCtlClockGet() / 2;  // Calcular el per�odo (1Hz)
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);  // Establecer el per�odo
    TimerEnable(TIMER0_BASE, TIMER_A);  // Habilitar el temporizador

    // Configuraci�n de las interrupciones del temporizador
    IntEnable(INT_TIMER0A);  // Habilitar la interrupci�n del temporizador
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // Habilitar la interrupci�n por timeout del temporizador
    IntMasterEnable();  // Habilitar las interrupciones globales

    // Configuraci�n del m�dulo UART
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));  // Configuraci�n del UART

    // Configuraci�n de las interrupciones del UART
    IntEnable(INT_UART0);  // Habilitar la interrupci�n del UART
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);  // Habilitar la interrupci�n por recepci�n del UART

    while (1) {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0xFF);  // Encender todos los LEDs
        delay(10000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);  // Apagar el LED rojo
        delay(10000);
    }
}

// Parte 3 - Funci�n de retardo
void delay(uint32_t ui32Ms) {
    SysCtlDelay(ui32Ms * (SysCtlClockGet() / 3 / 1000));
}

