/************************************************************************
 * 								pSat Project							*
 ************************************************************************
 *	  ^	  ___________________
 *	  |___|Vcc			 GND|
 *		  |P1.0			 XIN|
 *	MISO->|P1.1			XOUT|
 *	MOSI<-|P1.2			TEST|
 *		  |P1.3			 RST|
 *	 CLK<-|P1.4			P1.7|
 *		  |P1.5			P1.6|
 *	  CE<-|P2.0			P2.5|
 *	 CSN<-|P2.1			P2.4|
 *	 IRQ->|P2.2			P2.3|
 *		_____________________
 * Comments:
 * -	For RX uncomment SPI_DRIVER_USCI_B
 * -	For TX uncomment SPI_DRIVER_USCI_A
 *
 *
 *
 */

#include <msp430.h>
#include "msprf24.h"
#include "nrf_userconfig.h"
#include "stdint.h"
#include "pSat.h"
#include "tempConfig.h"

long tempAux;

int main(){

	WDTCTL = WDTHOLD | WDTPW;
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
	BCSCTL2 = DIVS_1;  // SMCLK = DCOCLK/2
	// SPI (USCI) uses SMCLK, prefer SMCLK < 10MHz (SPI speed limit for nRF24 = 10MHz)

	// Red, Green LED used for status
	P1DIR |= 0x41;
	P1OUT &= ~0x41;

	P2DIR |=BIT3;

	psat_InitAntenna(RF24_SPEED_1MBPS,RF24_POWER_0DBM,120);

	psat_OpenPipe(PIPE_NUMBER);

	tempInit();

	w_tx_addr(addr[PIPE_NUMBER]);
	w_rx_addr(PIPE_NUMBER, addr[PIPE_NUMBER]);

	while(1){
		P2OUT ^= BIT3;
		__delay_cycles(8000000);
		tempAux = getTemp();
		pac[0] = (tempAux/10) + '0';
		pac[1] = ((tempAux)%10) + '0';
		//if(++buf[0]=='i'){buf[0] = 'a';buf[1] = 'b';}
		//else {buf[1]++;}
		w_tx_payload(PACKET_SIZE, pac);
		msprf24_activate_tx();
		LPM4;

		if (rf_irq & RF24_IRQ_FLAGGED) {
			rf_irq &= ~RF24_IRQ_FLAGGED;

			msprf24_get_irq_reason();
			if (rf_irq & RF24_IRQ_TX){
				P1OUT &= ~BIT0; // Red LED off
				P1OUT |= 0x40;  // Green LED on
			}
			if (rf_irq & RF24_IRQ_TXFAILED){
				P1OUT &= ~BIT6; // Green LED off
				P1OUT |= BIT0;  // Red LED on
			}

			msprf24_irq_clear(rf_irq);
		}
	}
	return 0;
}
