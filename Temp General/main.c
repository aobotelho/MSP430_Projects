#include "msp430g2553.h"



int main(void)
{

	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	//MCLK = 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	//All pins but accelerometers output + Light up P1.0 LED
  	P1DIR |= BIT0;
  	P1OUT = BIT0;



	while(1)
	{
		__delay_cycles(1000000);
		P1OUT&= ~BIT0;
		__delay_cycles(1000000);
		P1OUT |= BIT0;
	}
}


