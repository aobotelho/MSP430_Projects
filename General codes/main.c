#include "interruptions_vectors.h"


void  main(void) {

    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= BIT6;
    P1OUT &= ~BIT6;

    UARTInit();

    __enable_interrupt();

    UARTVar.SendBuffer[0] = 0x40;
   	UARTVar.SendBuffer[1] = 0x41;
   	UARTVar.SendBuffer[2] = 0x42;
    UARTVar.SendBuffer[3] = 0x43;
    UARTVar.SendBuffer[4] = 0x44;
    UARTVar.SendBuffer[5] = 0x45;
    UARTVar.SendBuffer[6] = 0x46;
    UARTVar.SendBuffer[7] = 0x47;
    UARTVar.SendBuffer[8] = 0x48;
    UARTVar.SendBuffer[9] = 0x49;
    UARTVar.SendBuffer[10] = 0x50;
    UARTVar.SendBuffer[11] = 0x51;
    UARTVar.SendBuffer[12] = 0x52;
    UARTVar.SendBuffer[13] = 0x53;
    UARTVar.SendBuffer[14] = 0x54;
    UARTVar.SendBuffer[15] = 0x55;
    UARTVar.SendBuffer[16] = 0x56;
    UARTVar.SendBuffer[17] = 0x57;
    UARTVar.SendBuffer[18] = 0xFF;
    UARTVar.SendBuffer[19] = 0xFF;



    while(1){
    	UARTVar.BytesToBeSent= 20;
    	SEND_DATA;

    	P1OUT ^= BIT6;
    	__delay_cycles(1000);
    }
}
