#include "SPIConfig.h"

#if defined USE_SPI_USCIA
	void SPIInit_USCIA(void){

	}
#endif

#if defined USE_SPI_USCIB
	void SPIInit_USCIB(void){
		/* Configure ports on MSP430 device for USCI_B */
		P1SEL |= MOSIB | MISOB | SCKB;
		P1SEL2 |= MOSIB | MISOB | SCKB;

		// Configure SPI
		// Hold on Reset
		UCB0CTL1 |= UCSWRST;
		// Data is Captured on first CLK Edge (UCCKPH)
		// MSB first (UCMSB)
		// Master (UCMST)
		// 8-bit characters (UC7BIT not set)
		// 3-pin SPI (UCMODE_0)
		// Synchronous Mode (UCSYNC)
		UCB0CTL0 = UCCKPH | UCMSB | UCMST | UCMODE_0 | UCSYNC;  // SPI mode 0, master
		// Set divisor = 1 (SPIclock = SMCLK/1)
		UCB0BR0 = 0x01;
		UCB0BR1 = 0x00;
		// Clock = SMCLK (UCSSEL_2)
		// Clear UCSWRST (UCSWRST not set)
		UCB0CTL1 = UCSSEL_2;
	}

	unsigned char SPIB_transfer(char inb){
		#if defined SPI_USE_TX_INTERRUPT
			IE2 |= UCB0RXIE;
			UCB0TXBUF = inb;
			do {
				LPM0;
			} while (UCB0STAT & UCBUSY);
		#else
			UCB0TXBUF = inb;
			// Wait for RXIFG indicating remote byte received via SOMI
			while ( !(IFG2 & UCB0RXIFG) );
		#endif

		return UCB0RXBUF;
	}

	unsigned int SPIB_transfer16(int inw){
		int retw;

		#if defined SPI_USE_TX_INTERRUPT
			IE2 |= UCB0RXIE;
			UCB0TXBUF = (inw >> 8) & 0xFF;  // Send MSB first...
			do {
				LPM0;
			} while (UCB0STAT & UCBUSY);

			retw = UCB0RXBUF << 8;

			IE2 |= UCB0RXIE;
			UCB0TXBUF = inw & 0xFF;
			do {
				LPM0;
			} while (UCB0STAT & UCBUSY);

			retw |= UCB0RXBUF;
		#else
			UCB0TXBUF = (inw >> 8) & 0xFF;
			while ( !(IFG2 & UCB0RXIFG) );

			retw = UCB0RXBUF << 8;

			UCB0TXBUF = inw & 0xFF;
			while ( !(IFG2 & UCB0RXIFG) );

			retw |= UCB0RXBUF;
		#endif

		return retw;
	}

#endif
