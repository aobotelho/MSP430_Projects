#include "interruptions_vectors.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    DCOCTL = 0;

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= BIT0;
    P1OUT |= BIT0;

    UARTInit();

    __enable_interrupt();
    while(1){
    	SendUART("Hello!\n");
    	__delay_cycles(100);
    	P1OUT ^= BIT0;
    }
	return 0;
}
