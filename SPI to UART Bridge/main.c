#include <msp430.h>

#define MISO	BIT6
#define MOSI	BIT7
#define SCK		BIT5
#define SS		BIT0	// P2.0
#define RX		BIT1
#define TX		BIT2
#define LED		BIT0	// P1.0

unsigned char buff='x';

int main(void){
	WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
	// 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P1SEL |= MOSI | MISO | SCK | RX | TX;
	P1SEL2 |= MOSI | MISO | SCK | RX | TX;

	// Set LED as output
	P1DIR |= LED;
	P1OUT |= LED;
	// SS as input
	P2DIR &= ~SS;

	// USCIA - UART
	UCA0CTL1 |= UCSWRST;
	//Prescalers (Prescaler = (UCA0BR0 + UCA0BR1 x 256));
	UCA0BR0 = 104;
	UCA0BR1 = 0x00;
	//Second modulation stage. See page 431, table 15-2 and 15-3 on MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf).
	UCA0MCTL = UCBRS0;
   	//Control Register 1; see page 430 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
   	UCA0CTL1 =
   			  UCSSEL_3		// CLK source; 0 - UCLK ; 1 - ACLK ; 2 - SMCLK ; 3 - SMLCK
	    	;
   	UCA0CTL1 &= ~UCSWRST;

	//USCIB - SPI
	//Data change on the First edge - ~UCCKPH
	//Clock inactive state is low- ~UCCKPL
	//LSB - ~UCMSB
	//8-bit data - ~UC7BIT
	//Slave - ~UCMST
	//3-pin SPI - UCMODE_0
	//Synchronous - UCSYNC
	UCB0CTL1 = UCSWRST;
	UCB0CTL0 = (UCB0CTL0 & ~(UCMST | UC7BIT | UCCKPH | UCMSB | UCCKPL))  | UCSYNC  | UCMODE_0  ;
	UCB0CTL1 &= ~UCSWRST;
	IE2 |= UCB0RXIE;



	__bis_SR_register(LPM4_bits + GIE);       // Enter LPM4, enable interrupts
	}

// Echo character
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR (void){
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  P1OUT ^= BIT0;
  UCA0TXBUF = UCB0RXBUF;
  UCB0TXBUF = 'c';
  buff= UCB0RXBUF;
  //__delay_cycles(50);
}
