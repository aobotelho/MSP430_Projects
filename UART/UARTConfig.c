#include "UARTConfig.h"

#if defined USE_UART

void UARTInit(){

	// Hold UART in Reset
	UCA0CTL1 |= UCSWRST;
	// TX and RX
    P1SEL |= UART_RX + UART_TX;
    P1SEL2 |= UART_RX + UART_TX;

    //Prescalers (Prescaler = (UCA0BR0 + UCA0BR1 x 256)); see page 430 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
    UCA0BR0 = 8;
    UCA0BR1 = 0x00;

	//Second modulation stage. See page 431, table 15-2 and 15-3 on MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf).
   	UCA0MCTL = 0x0A;

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
//				UCCKPH			// If set: Data is captured on the first UCLK edge and changed on the following edge. If not set: changed on the first edge and captured on the following
//				UCCKPL			// If set: Inactive state high. If not set: Inactive state low.
//				UCMSB 			// If set: MSB first. If not set: LSB first
//				| UC7BIT		// If set: 7-bit data. If not set: 8-bit data
//				| UCMST 		// If set: Master. If not set: Slave
//				| UCMODE_X		// 0 - 3 pin SPI ; 1 - 4 pin SPI & slave enabled when UCxSTE = 1 ; 2 - 4 pin SPI & slave enabled when UCxSTE = 0 ; 3 - I²C mode;
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

void SendUART(char* message){
	UARTVar.BytesToBeSent=0;

	do{
		UARTVar.SendBuffer[UARTVar.BytesToBeSent] = *message++;
	}while(UARTVar.SendBuffer[UARTVar.BytesToBeSent++] != '\0');
	UARTVar.BytesToBeSent--;
	SEND_DATA;
}
#endif

