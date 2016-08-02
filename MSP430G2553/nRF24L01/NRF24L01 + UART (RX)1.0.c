#include <msp430.h>
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Spirilis\msprf24.c"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Spirilis\nrf_userconfig.h"
#include "stdint.h"

volatile unsigned int user;

//*************************************************************************
#define Size 7
unsigned int tx_count = 0;
unsigned char send[Size];

//*************************************************************************

void main()
{
	char addr[5];
	char buf[32];

	WDTCTL = WDTHOLD | WDTPW;
	DCOCTL = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;

//***********************************************************************
//	BCSCTL2 = DIVS_0;  // SMCLK = DCOCLK/2
//***********************************************************************

	// SPI (USCI) uses SMCLK, prefer SMCLK < 10MHz (SPI speed limit for nRF24 = 10MHz)

	// Red LED will be our output
	P1DIR |= BIT0;
	P1OUT &= ~(BIT0);

	P2DIR |= BIT3;
	P2OUT &= ~BIT3;


//****************************************************************************
	//UART configuration
	    	UCA0CTL1 |=	UCSWRST;
	    	//Set RX and TX
	    	P1SEL  |= BIT1 + BIT2;
	    	P1SEL2 |= BIT1 + BIT2;

	    	//Set register to a Baud Rate of 9600 (10^6/9600 = 104)
	    	UCA0BR0 |= 104;
	    	UCA0BR1 |= 0;
	    	//Second stage modulation 1 (UCBRS_1)
	    	UCA0MCTL |= UCBRS0;
	    	//SMCLK (UCSSEL_2), Start USCI (&=~UCSWRST)
	    	UCA0CTL1 |= UCSSEL_2;
	    	UCA0CTL1  &= ~UCSWRST;

	    	strcpy(send, "Hello\r\n\n");
	    	IE2 |= UCA0TXIE;

//**********************************************************************************************


	user = 0xFE;

	/* Initial values for nRF24L01+ library config variables */
	rf_crc = RF24_EN_CRC | RF24_CRCO; // CRC enabled, 16-bit
	rf_addr_width      = 5;
	rf_speed_power     = RF24_SPEED_1MBPS | RF24_POWER_0DBM;
	rf_channel         = 120;

	msprf24_init();
	msprf24_set_pipe_packetsize(0, 32);
	msprf24_open_pipe(0, 1);  // Open pipe#0 with Enhanced ShockBurst

	// Set our RX address
	addr[0] = 0xDE;	addr[1] = 0xAD;	addr[2] = 0xBE;	addr[3] = 0xEF;	addr[4] = 0x00;
	w_rx_addr(0, addr);

	// Receive mode
	if (!(RF24_QUEUE_RXEMPTY & msprf24_queue_state())) {
		flush_rx();
	}
	msprf24_activate_rx();
	LPM4;

	while (1) {
		if (rf_irq & RF24_IRQ_FLAGGED) {
			rf_irq &= ~RF24_IRQ_FLAGGED;
			msprf24_get_irq_reason();
		}
		if (rf_irq & RF24_IRQ_RX || msprf24_rx_pending()) {
			r_rx_payload(32, buf);
			msprf24_irq_clear(RF24_IRQ_RX);
			user = buf[0];

			if (buf[0] == '0'){
				P1OUT &= ~BIT0;
				strcpy(send, "Green\r\n");
			}
			if (buf[0] == '1')
				P1OUT |= BIT0;
			if (buf[1] == '0'){
				P2OUT &= ~BIT3;
				strcpy(send, "Red  \r\n");
			}
			if (buf[1] == '1')
				P2OUT |= BIT3;

		} else {
			user = 0xFF;
		}
		IE2 |= UCA0TXIE;
		LPM4;
	}
}


#pragma vector=USCIAB0TX_VECTOR
__interrupt void UART_interrupt(void){
    if(tx_count<=Size)	UCA0TXBUF = send[ tx_count++ ];
    else{
    	IE2 &= ~UCA0TXIE;
    	tx_count=0;
    }

}
