#include "msp430g2553.h"
#include "stdint.h"
#include "interruptions_vectors.h"

int main(void){

	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	//MCLK = 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;


	//-----------------------------------------------------
	//Inputs & Outputs
	//-----------------------------------------------------
	P2DIR |= LED1_IO+LED2_IO+LED3_IO;
	LED1_PORT &= ~LED1_IO;
	LED2_PORT &= ~LED2_IO;
	LED3_PORT &= ~LED3_IO;


  	//*****************************************************
  	//Defines variables for the nRF library
  	//*****************************************************
  	// Enable CRC (_EN_CRC), 2-byte long (_CRC0)
  	// Other options: Disable CRC (~_EN_CRC) or 1-byte long CRC (~_CRC0)
	rf_crc = RF24_EN_CRC | RF24_CRCO;

	// Address Width = 5 bytes
  	// Other options: 3 or 4 bytes;
	rf_addr_width = 5;

	// nRF Speed = 1Mbps (_1MBPS)
  	// Other options: 250Kbps (_250KBPS) or 2Mbps (_2MBPS)
	// Power = 0 dBm
	// Other options: -60dBm (_MINUS6DBM), -12dBm (_MINUS12DBM) or -18dBm (_MINUS18DBM)
	rf_speed_power = RF24_SPEED_1MBPS | RF24_POWER_0DBM;

	// RF channel = 20
	// Possible values [0,125] (Check Legal ISM bands in your country)
	rf_channel = 20;


	//*****************************************************
	//initialize nFR ; configure and open pipes ;
	//*****************************************************
	msprf24_init();
	msprf24_set_pipe_packetsize(2,12);
	msprf24_open_pipe(2, 0);  // Open pipe#0 with Enhanced ShockBurst


	//*****************************************************
	//Receiver address (must be equal to the transmitter)
	//*****************************************************
	w_rx_addr(1, (char*)addr_pipe2);


	//*****************************************************
	//Flush RX to ensure there's nothing left from previous usage
	//*****************************************************
	flush_rx();

	//*****************************************************
	//Main loop
	//*****************************************************
	while(1){
		//*****************************************************
		//Initiate RX and sleep ; wait until IRQ is low
		//*****************************************************
		msprf24_activate_rx();
		LPM4;


		//*****************************************************
		//Check if TX or TXFAILED flags are up
		//*****************************************************
		if (rf_irq & RF24_IRQ_FLAGGED) {

			msprf24_get_irq_reason();

			if (rf_irq & RF24_IRQ_TX) 		LEDTX_PORT ^= LEDTX_IO;

			if (rf_irq & RF24_IRQ_TXFAILED)	LEDTXFAILED_PORT ^= LEDTXFAILED_IO;

			if (rf_irq & RF24_IRQ_RX)	LEDRX_PORT ^= LEDRX_IO;

			msprf24_irq_clear(RF24_IRQ_MASK);
		}


		//*****************************************************
		//If IRQ RX flag is up enter main loop
		//*****************************************************
		if (msprf24_rx_pending()){

				//*****************************************************
				//Store the RX data on the buf variable
				//*****************************************************
				//r_rx_payload(vector_size, buf);

		}

			//*****************************************************
			//Set the interruption ; put MSP to sleep
			//*****************************************************
			//IE2 = UCA0TXIE;
			//__bis_SR_register(CPUOFF + GIE);


			//*****************************************************
			//Check if RX register is full ; if it is, flush
			//*****************************************************
			if(msprf24_queue_state()&RF24_QUEUE_RXFULL) flush_rx();

	}
}



