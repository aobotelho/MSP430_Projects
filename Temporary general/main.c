#include "interruptions_vectors.h"


void  main(void) {

    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= BIT0 | BIT6;
    P1DIR &= ~BIT3;
    P1OUT |= BIT0 | BIT6;

    TACTL = TASSEL_2 | ID_3 | MC_1 ;
    TACCR0 = 0xF424;

    __enable_interrupt();

    while(1){
    	if(!(P1IN & BIT3)){
    		P1OUT |= BIT0;
    		while(!(P1IN & BIT3));
    	}
    	else{
    		P1OUT &= ~BIT0;
    		while(P1IN & BIT3);
    	}
    }
}
