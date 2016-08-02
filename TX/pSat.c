#include "pSat.h"

void psat_InitAntenna(uint8_t speed, uint8_t power, uint8_t channel){
	unsigned int counter = 0;

	for(;counter < 6; counter++){
		addr[counter][0] = ADDR_BYTE1;
		addr[counter][1] = ADDR_BYTE2;
		addr[counter][2] = ADDR_BYTE3;
		addr[counter][3] = ADDR_BYTE4;
	}

	addr[0][4] = ADDR_PIPE0;
	addr[1][4] = ADDR_PIPE1;
	addr[2][4] = ADDR_PIPE2;
	addr[3][4] = ADDR_PIPE3;
	addr[4][4] = ADDR_PIPE4;
	addr[5][4] = ADDR_PIPE4;



	rf_crc = RF24_EN_CRC | RF24_CRCO; // CRC enabled, 16-bit
	rf_addr_width      = ADDR_SIZE;
	rf_speed_power     = speed | power;
	rf_channel         = channel;

	msprf24_init();  // All RX pipes closed by default
}

void psat_OpenPipe(uint8_t pipe){
	msprf24_set_pipe_packetsize(pipe, BUFFER_SIZE);
	msprf24_open_pipe(pipe, AUTO_ACK_ON);
	msprf24_standby();
}
