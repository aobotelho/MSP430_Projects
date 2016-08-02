#include "UARTConfig.h"

void UARTInit(){
	unsigned int auxCounter=0;

	for(;auxCounter < UART_BUFFER_SIZE; auxCounter++)
		UARTVar.SendBuffer[auxCounter] = 'p';

	/*UARTVar.BufferEnd = UARTVar.SendBuffer;
	UARTVar.BufferInit = UARTVar.SendBuffer;*/
	UARTVar.bufferInit = 0;
	UARTVar.bufferEnd = 0;



	// Hold UART in Reset
	UCA0CTL1 |= UCSWRST;
	// TX and RX
    P1SEL  |= UART_RX + UART_TX;
    P1SEL2 |= UART_RX + UART_TX;

    //Prescalers (Prescaler = (UCA0BR0 + UCA0BR1 x 256)); see page 430 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
    //SMCLK = 8MHZ ; 8M/9600 = ~833
    UCA0BR0 = 0x41;
    UCA0BR1 = 0x03;

	//Second modulation stage. See page 431, table 15-2 and 15-3 on MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf).
   	UCA0MCTL = 0x04;

   	//Control Register 1; see page 430 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
   	UCA0CTL1 =
   			  UCSSEL_2		// CLK source; 0 - UCLK ; 1 - ACLK ; 2 - SMCLK ; 3 - SMLCK
// 			  | UCRXEIE		// Enable to set UCAxRXIFG when an erroneous character is received
//	 		  | UCBRKIE		// Receive break enabled
// 			  | UCDORM		// Put USCI on dormant mode
// 			  | UCTXADDR	// Transmit address; If set next frame transmitted is an address
//		  	  | UCTXBRK		// Transmit break; If set next frame transmitted is a break
	    	;

   	//Control Register 0; see page 430 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
//	UCA0CTL0 =
//				UCPEN			// If set: Parity enabled
//				UCPAR			// If set: Even parity. If not set: Odd parity
//				UCMSB 			// If set: MSB first. If not set: LSB first
//				| UC7BIT		// If set: 7-bit data. If not set: 8-bit data
//				| UCSPB 		// If set: Two stop bit. If not set: One stop bit
//				| UCMODE_X		// 0 - UART mode ; 1 - Idle-line multiprocessor mode; 2 - Address-bit multiprocessor mode; 3 - UART mode with automatic baud rate detection;
//				| UCSYNC		// If set: synchronous mode. If not set: Asynchronous mode
//	  	  			 ;


	//Status Register; see page 431 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
// 	UCA0STAT =
//				UCLISTEN 		// If set: Listen enabled (transmitter output fed back to the receiver). If not set: Listen disabled
// 	  			| UCFE			// If set indicates that a character was received with a low stop bit
//	  			| UCOE			// Must NOT be set by software. If set indicates that a character was received in the UCxRXBUF before the previous character was read.
//				| UCPE			// If set a character was received with a parity error
// 	  			| UCBUSY		// If set indicates that USCI is transmitting or receiving.
// 	  			;

	UCA0CTL1 &= ~UCSWRST;
}

void AddToUARTBuffer(uint8_t message[BUFFER_SIZE]){
	unsigned int counter=0;

	for(;counter<BUFFER_SIZE;counter++){
		UARTVar.SendBuffer[UARTVar.bufferEnd] = message[counter];
		if(++UARTVar.bufferEnd == UART_BUFFER_SIZE)
			UARTVar.bufferEnd = 0;
	}

}

