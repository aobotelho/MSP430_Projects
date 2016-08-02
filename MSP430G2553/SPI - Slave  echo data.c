/********************************************************************
 * Code designed for the MSP430G2553 on the LaunchPad
 * Uses USCI A RX vector to echo the data received by SPI
 * This is the Slave side
 * The data received can be checked on the received[] variable
 */


//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          | |             XOUT|-
// Master---+-|RST              |
//            |             P1.2|<- Data Out (UCA0SOMI)
//            |                 |
//            |             P1.1|-> Data In (UCA0SIMO)
//            |                 |
//            |             P1.4|<- Serial Clock In (UCA0CLK)
//
#include <msp430.h>

unsigned char received [200];
long int i=0;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  P1SEL = BIT1 + BIT2 + BIT4;
  P1SEL2 = BIT1 + BIT2 + BIT4;
  UCA0CTL1 = UCSWRST;
  P1DIR |= BIT0;
  P1OUT |= BIT0;
  //Data change on the First edge - UCCKPH
  //Clock inactive state is high - UCCKPL
  //LSB - UCMSB
  //8-bit data - UC7BIT
  //Slave - UCMST
  //3-pin SPI - UCMODE_0
  //Synchronous - UCSYNC
  UCA0CTL0 = (UCA0CTL0 & ~(UCMST | UC7BIT | UCCKPH | UCMSB))  | UCSYNC | UCCKPL | UCMODE_0  ;
  UCA0CTL1 &= ~UCSWRST;
  IE2 |= UCA0RXIE;

  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4, enable interrupts
}

// Echo character
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR (void)
{
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  P1OUT ^= BIT0;
  UCA0TXBUF = UCA0RXBUF;
  //__delay_cycles(50);
}
