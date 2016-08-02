#ifndef UARTCONFIG_H_
#define UARTCONFIG_H_

#include "Hardware.h"

#if defined USE_UART

	#define UART_BUFFER_SIZE	32

	typedef struct{
		unsigned char SendBuffer[UART_BUFFER_SIZE];
		unsigned char ReceiveBuffer[UART_BUFFER_SIZE];
		unsigned int SendByteCounter;
		unsigned int BytesToBeSent;
		unsigned int BytesReceived;
		unsigned int NewData;
	}UART_VAR;

	UART_VAR UARTVar;

	#define	SEND_DATA	{IE2|=UCA0TXIE;__bis_SR_register(CPUOFF);}

	void UARTInit(void);
	void SendUART(char* message);



#endif // USE_UART
#endif // UARTCONFIG_H_
