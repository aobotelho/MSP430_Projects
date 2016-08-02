#include "msp430g2553.h"
#include "stdint.h"
#include "interruptions_vectors.h"


int main(void){
	// Stop WDT
	WDTCTL = WDTPW + WDTHOLD;

	//-----------------------------------------------------
	//Inputs & Outputs
	//-----------------------------------------------------
	LED0_DIR |= LED0_IO;
	LED1_DIR |= LED1_IO;
	LED2_DIR |= LED2_IO;

  	LED0_PORT &= ~LED0_IO;
  	LED1_PORT &= ~LED1_IO;
	LED2_PORT &= ~LED2_IO;

	Home_Initialize();
	TimerAInit();
	//-----------------------------------------------------
	//Main loop
	//-----------------------------------------------------
	while(1){
		Home_SendCommand(PIPE_1,IsAlive,0x04);
		__delay_cycles(100000);
	}
}



