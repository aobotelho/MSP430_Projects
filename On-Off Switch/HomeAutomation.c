#include "HomeAutomation.h"

#if defined HOME_AUTOMATION

void Home_SetClock(void){
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
}

void Home_ConfigAntenna(void){
  	//-----------------------------------------------------
  	//Defines variables for the nRF library
  	//-----------------------------------------------------
  	// Enable CRC (_EN_CRC), 2-byte long (_CRC0)
  	// Other options: Disable CRC (~_EN_CRC) or 1-byte long CRC (~_CRC0)
	rf_crc = RF24_EN_CRC | RF24_CRCO;

	// Address Width = 5 bytes
	rf_addr_width = ADDR_SIZE;

	// nRF Speed = 1Mbps (_1MBPS)
  	// Other options: 250Kbps (_250KBPS) or 2Mbps (_2MBPS)
	// Power = 0 dBm
	// Other options: -60dBm (_MINUS6DBM), -12dBm (_MINUS12DBM) or -18dBm (_MINUS18DBM)
	rf_speed_power = RF24_SPEED_1MBPS | RF24_POWER_0DBM;

	// RF channel
	rf_channel = RF_CHANNEL;

	msprf24_init();

	msprf24_standby();
}

void Home_SetPipes(void){
#if defined CENTRAL_NODE
	unsigned int i=0;
	for(;i<6;i++)	msprf24_set_pipe_packetsize(i,PACKET_SIZE);
	w_rx_addr(PIPE_0, (char*)addr_pipe0);
	w_rx_addr(PIPE_1, (char*)addr_pipe1);
	w_rx_addr(PIPE_2, (char*)addr_pipe2);
	w_rx_addr(PIPE_3, (char*)addr_pipe3);
	w_rx_addr(PIPE_4, (char*)addr_pipe4);
	w_rx_addr(PIPE_5, (char*)addr_pipe5);
#endif

#if defined ON_OFF_SWITCH
	msprf24_set_pipe_packetsize(PIPE_1,PACKET_SIZE);
	w_rx_addr(PIPE_1, (char*)addr_pipe1);
#endif
}

void Home_Initialize(void){
	Home_SetClock();
	Home_ConfigAntenna();
	Home_SetPipes();
	TXAttempt = 1;
}


void Home_OpenPipe(Pipe_Num Pipe){
	msprf24_open_pipe(Pipe, 0);
	switch(Pipe){
		case PIPE_0:
			w_tx_addr((char*)addr_pipe0);
			break;
		case PIPE_1:
			w_tx_addr((char*)addr_pipe1);
			break;
		case PIPE_2:
			w_tx_addr((char*)addr_pipe2);
			break;
		case PIPE_3:
			w_tx_addr((char*)addr_pipe3);
			break;
		case PIPE_4:
			w_tx_addr((char*)addr_pipe4);
			break;
		case PIPE_5:
			w_tx_addr((char*)addr_pipe5);
			break;
	}
}
void Home_ClosePipe(Pipe_Num Pipe){
	msprf24_close_pipe(Pipe);
}


void Home_SendCommand(Pipe_Num Pipe,Commands command, ID id){

	Home_OpenPipe(Pipe);

	//-----------------------------------------------------
	// Flush TX to ensure there's nothing left from previous usage
	//-----------------------------------------------------
	flush_tx();

	//-----------------------------------------------------
	// Configure packet correctly
	//-----------------------------------------------------
	TXBuffer[0] = command;
	TXBuffer[1] = id;
	TXBuffer[2] = TXAttempt + 0x20;

	//-----------------------------------------------------
	// Send Data and go to sleep
	//-----------------------------------------------------
	w_tx_payload(PACKET_SIZE,(char*)TXBuffer);
	msprf24_activate_tx();
	LPM4;

	//-----------------------------------------------------
	//Check if TX or TXFAILED or RX flags are up
	//-----------------------------------------------------
	if (rf_irq & RF24_IRQ_FLAGGED) {
		do{
			msprf24_irq_clear(RF24_IRQ_MASK);
			flush_rx();
			msprf24_activate_rx();
			LPM4;
			r_rx_payload(PACKET_SIZE, (char*)RXBuffer);
			if(RXBuffer[0] == Answer && RXBuffer[1] == CENTRAL_NODE_ID)	TXAttempt=0;
		}while(TXAttempt);
	}

	//-----------------------------------------------------
	//Clear IRQ mask
	//-----------------------------------------------------
	msprf24_irq_clear(RF24_IRQ_MASK);

	Home_ClosePipe(Pipe);
}


void Home_SendResponse(void){
	//-----------------------------------------------------
	// Flush TX to ensure there's nothing left from previous usage
	//-----------------------------------------------------
	flush_tx();

	//-----------------------------------------------------
	// Configure packet correctly
	//-----------------------------------------------------
	TXBuffer[0] = Answer;
	TXBuffer[1] = CENTRAL_NODE_ID;
	TXBuffer[2] = TXAttempt + 0x20;

	//-----------------------------------------------------
	// Send Data and go to sleep
	//-----------------------------------------------------
	w_tx_payload(PACKET_SIZE,(char*)TXBuffer);
	msprf24_activate_tx();
	LPM4;

	//-----------------------------------------------------
	//Clear IRQ mask
	//-----------------------------------------------------
	msprf24_irq_clear(RF24_IRQ_MASK);

	TXAttempt = 0;
}


void Home_WaitCommand(void){
#if defined ON_OFF_SWITCH
	Home_OpenPipe(PIPE_1);
	flush_rx();
	msprf24_activate_rx();
	LPM4;

	// received something.....toggle LED2
	LED2_PORT ^= LED2_IO;


	//-----------------------------------------------------
	//Check if TX or TXFAILED or RX flags are up
	//-----------------------------------------------------
	if (rf_irq & RF24_IRQ_FLAGGED) {
		do{
			msprf24_irq_clear(RF24_IRQ_MASK);
			r_rx_payload(PACKET_SIZE, (char*)RXBuffer);
			if(RXBuffer[1] == MY_ID){
				switch(RXBuffer[0]){
				case IsAlive:
					if(LED0_PORT & LED0_IO){
						TXBuffer[3] = '1';
					}
					else{
						TXBuffer[3] = '0';
					}
					Home_SendResponse();
					//Sent response....Toggle LED1
					LED1_PORT ^= LED1_IO;
					break;
				case OutputValue:
					if(LED0_PORT & LED0_IO) TXBuffer[3] = '1';
					else	TXBuffer[3] = '0';
					Home_SendResponse();
					break;
				}
			}
			else{
				TXAttempt=0;
			}
		}while(TXAttempt);
	}
#endif
}

#endif	/*HOME_AUTOMATION*/
