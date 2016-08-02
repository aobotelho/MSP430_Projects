#include "interruptions_vectors.h"

const char send[12] = {'0',4,'0','0','0','0','0','0','0','0','0','0'};

int main(void){
	// Stop WDT
	WDTCTL = WDTPW + WDTHOLD;
	//-----------------------------------------------------
	// Define Clock
	//-----------------------------------------------------
	#if defined CLOCK_1MHZ
	#if defined CLOCK_8MHZ
		#pragma CLOCK_CONFLICT
	#else
		#if defined CLOCK_16MHZ
			#pragma CLOCK_CONFLICT
		#else
			// No clock conflict
			BCSCTL1 = CALBC1_1MHZ;
			DCOCTL = CALDCO_1MHZ;
		#endif
	#endif
	#else
	#if defined CLOCK_8MHZ
		#if defined CLOCK_16MHZ
			#pragma CLOCK_CONFLICT
		#else
			// No clock conflict
			BCSCTL1 = CALBC1_8MHZ;
			DCOCTL = CALDCO_8MHZ;
		#endif
	#else
		#if defined CLOCK_16MHZ
			// No clock conflict
			BCSCTL1 = CALBC1_16MHZ;
			DCOCTL = CALDCO_16MHZ;
		#else
			#pragma DEFINE_CLOCK
		#endif
	#endif
	#endif

	//-----------------------------------------------------
	//Inputs & Outputs
	//-----------------------------------------------------
	LED0_DIR |= LED0_IO;
	LED1_DIR |= LED1_IO;
	LED2_DIR |= LED2_IO;

	LED0_PORT &= ~LED0_IO;
	LED1_PORT &= ~LED1_IO;
	LED2_PORT &= ~LED2_IO;


	//-----------------------------------------------------
	//Defines variables for the nRF library
	//-----------------------------------------------------
	// Enable CRC (_EN_CRC), 2-byte long (_CRC0)
	// Other options: Disable CRC (~_EN_CRC) or 1-byte long CRC (~_CRC0)
	rf_crc = RF24_EN_CRC | RF24_CRCO;

	// Address Width = 5 bytes
	// Defined on HomeAutomation.h
	rf_addr_width = ADDR_SIZE;

	// nRF Speed = 1Mbps (_1MBPS)
	// Other options: 250Kbps (_250KBPS) or 2Mbps (_2MBPS)
	// Power = 0 dBm
	// Other options: -60dBm (_MINUS6DBM), -12dBm (_MINUS12DBM) or -18dBm (_MINUS18DBM)
	rf_speed_power = RF24_SPEED_1MBPS | RF24_POWER_0DBM;

	// RF channel
	// Defined on HomeAutomation.h
	rf_channel = RF_CHANNEL;

	//-----------------------------------------------------
	// Initialize nFR ; configure and open pipes ;
	//-----------------------------------------------------
	msprf24_init();
	msprf24_set_pipe_packetsize(PIPE_1,PACKET_SIZE);
	msprf24_open_pipe(PIPE_1,0);  // Open pipe#0 with Enhanced ShockBurst enabled

	//-----------------------------------------------------
	//Receiver address (must be equal to the transmitter)
	//-----------------------------------------------------
	msprf24_standby();
	w_rx_addr(PIPE_1, (char*)addr_pipe1);
	w_tx_addr((char*)addr_pipe1);

	//*****************************************************
	//Main loop
	//*****************************************************
	while(1){
		//*****************************************************
		//Flush TX to ensure there's nothing left from previous usage
		//*****************************************************
		flush_tx();


		//*****************************************************
		//Load TX register with the "send" vector ; put MSP to sleep until IRQ is low
		//*****************************************************
		w_tx_payload(PACKET_SIZE,(char*)send);
		msprf24_activate_tx();
		LPM4;

		LED0_PORT ^= LED0_IO;
		//*****************************************************
		//Check if TX or TXFAILED or RX flags are up
		//*****************************************************
		if (rf_irq & RF24_IRQ_FLAGGED) {
			msprf24_get_irq_reason();

			if (rf_irq & RF24_IRQ_TX){
				msprf24_irq_clear(RF24_IRQ_MASK);
				flush_rx();
				msprf24_activate_rx();
				LPM4;
				r_rx_payload(PACKET_SIZE, RXBuffer);
				if(RXBuffer[1] == 4) LED1_PORT ^= LED1_IO;
			}

			//*****************************************************
			//Clear IRQ mask
			//*****************************************************
			msprf24_irq_clear(RF24_IRQ_MASK);


			//*****************************************************
			//Arbitrary delay
			//*****************************************************
			__delay_cycles(500000);

			}

	 }
}
