/************************************************************************
 * 								pSat Project							*
 ************************************************************************
 *	  ^	  ___________________
 *	  |___|Vcc			 GND|
 *		  |P1.0			 XIN|
 *	  RX->|P1.1			XOUT|
 *    TX<-|P1.2			TEST|
 *		  |P1.3			 RST|
 *	 -|P1.4			P1.7|
 *		  |P1.5			P1.6|
 *	 -|P2.0			P2.5|
 *	 -|P2.1			P2.4|
 *	-|P2.2			P2.3|
 *		_____________________
 *
 * Comments:
 * -	For RX uncomment SPI_DRIVER_USCI_B in "nrf_userconfig.h"
 * -	For TX uncomment SPI_DRIVER_USCI_A in "nrf_userconfig.h"
 *
 *
 *Log:
 * - 20/07/2015: Check if nRF still receives data while IRQ pin is high
 *
 *
 */

#include <msp430.h>
#include "msprf24.h"
#include "nrf_userconfig.h"
#include "stdint.h"
#include "pSat.h"
#include "UARTConfig.h"

volatile unsigned int user;

int main(){
	WDTCTL = WDTHOLD | WDTPW;
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
	BCSCTL2 = DIVS_1;
	P1DIR |= BIT0+BIT4;
	P1OUT &= ~(BIT0);
	P1OUT |= BIT4;
	P2DIR |= BIT4;


	psat_InitAntenna(RF24_SPEED_1MBPS,RF24_POWER_0DBM,120);

	psat_OpenPipe(PIPE_NUMBER);

	w_rx_addr(PIPE_NUMBER, addr[PIPE_NUMBER]);

	UARTInit();

	// Receive mode
	if (!(RF24_QUEUE_RXEMPTY & msprf24_queue_state())) {
		flush_rx();
	}

	msprf24_activate_rx();

	while (1){
		if (rf_irq & RF24_IRQ_FLAGGED) {
			rf_irq &= ~RF24_IRQ_FLAGGED;
			msprf24_get_irq_reason();
		}
		if (rf_irq & RF24_IRQ_RX || msprf24_rx_pending()) {
			buf[BUFFER_SIZE-3] = r_rx_payload(PACKET_SIZE, buf) + 48;
			AddToUARTBuffer(buf);
			msprf24_irq_clear(RF24_IRQ_RX);
		}

		do{
			while (!(IFG2&UCA0TXIFG));
			if(UARTVar.bufferInit != UARTVar.bufferEnd)
				UCA0TXBUF = UARTVar.SendBuffer[UARTVar.bufferInit++];
			if(UARTVar.bufferInit == UART_BUFFER_SIZE)
				UARTVar.bufferInit = 0;
		}while(!(rf_irq & RF24_IRQ_FLAGGED));

	}
	return 0;
}

#pragma vector = PORT2_VECTOR
__interrupt void P2_IRQ (void) {
	P1OUT ^= BIT0;
	P2OUT ^= BIT4;
	if(P2IFG & nrfIRQpin) {
		rf_irq |= RF24_IRQ_FLAGGED;
		P2IFG &= ~nrfIRQpin;   // Clear interrupt flag
	}
}

