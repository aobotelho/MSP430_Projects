#ifndef RX_TEMPERATURE_UARTCONFIG_H_
#define RX_TEMPERATURE_UARTCONFIG_H_

#include <msp430.h>
#include <stdint.h>
#include "pSat.h"

#ifndef UART_BUFFER_SIZE
	#define UART_BUFFER_SIZE	32
#endif

#define UART_RX			BIT1
#define UART_TX			BIT2

typedef struct{
	volatile uint8_t SendBuffer[UART_BUFFER_SIZE];
	volatile uint8_t bufferInit;
	volatile uint8_t bufferEnd;
}UART_VAR;

UART_VAR UARTVar;

void UARTInit(void);
void AddToUARTBuffer(uint8_t message[BUFFER_SIZE]);


#endif // RX_TEMPERATURE_UARTCONFIG_H_
