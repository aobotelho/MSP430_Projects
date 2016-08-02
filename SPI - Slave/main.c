#include <msp430.h>

#define MISO	BIT6
#define MOSI	BIT7
#define SCK		BIT5
#define SS		BIT0	// P2.0
#define LED		BIT0	// P1.0

int main(void){
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  P1SEL |= MOSI | MISO | SCK ;
  P1SEL2 |= MOSI | MISO | SCK ;

  // Set LED as output
  P1DIR |= LED;
  P1OUT |= LED;
  // SS as input
  P2DIR &= ~SS;

  //USCIB - SPI
  //Data change on the First edge - ~UCCKPH
  //Clock inactive state is low - ~UCCKPL
  //LSB - ~UCMSB
  //8-bit data - ~UC7BIT
  //Slave - UCMST
  //3-pin SPI - UCMODE_0
  //Synchronous - UCSYNC
  UCB0CTL1 = UCSWRST;
  UCB0CTL0 = (UCB0CTL0 & ~(UCMST | UC7BIT | UCCKPH | UCMSB | UCCKPL))  | UCSYNC | UCMODE_0  ;
  UCB0CTL1 &= ~UCSWRST;
  IE2 |= UCB0RXIE;



  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4, enable interrupts
}

// Echo character
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR (void){
  while (!(IFG2 & UCB0TXIFG));              // USCI_A0 TX buffer ready?
  P1OUT ^= BIT0;
  UCB0TXBUF = UCB0RXBUF;
  //__delay_cycles(50);
}
