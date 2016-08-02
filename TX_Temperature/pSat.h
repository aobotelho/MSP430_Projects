#ifndef PSAT_H_
#define PSAT_H_

#include <msp430.h>
#include "msprf24.h"

// Definitions
#define PIPE_NUMBER	1
#define AUTO_ACK_ON	0

#define ADDR_SIZE	5
#define ADDR_BYTE1	0xDE
#define ADDR_BYTE2	0xAD
#define ADDR_BYTE3	0xBE
#define ADDR_BYTE4	0xEF
#define ADDR_PIPE0	0x00
#define ADDR_PIPE1	0x32
#define ADDR_PIPE2	0x33
#define ADDR_PIPE3	0x34
#define ADDR_PIPE4	0x35
#define ADDR_PIPE5	0x36

#define PACKET_SIZE	2
#define BUFFER_SIZE	PACKET_SIZE+3

#define RF_CHANNEL	120

// Variables Declarations
uint8_t addr[6][ADDR_SIZE];
uint8_t pac[PACKET_SIZE];
uint8_t buf[BUFFER_SIZE];

// Functions Declarations
void psat_InitAntenna(uint8_t speed, uint8_t power, uint8_t channel);
void psat_OpenPipe(uint8_t pipe);

#endif /* PSAT_H_ */
