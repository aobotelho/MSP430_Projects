#ifndef HOMEAUTOMATION_H_
#define HOMEAUTOMATION_H_

#include "Hardware.h"

#if defined HOME_AUTOMATION

//-----------------------------------------------------
// Protocol
//-----------------------------------------------------
#define BROADCAST_SIZE	3
#define BROADCAST_DIV	0xFF
#define	BROADCAST_HELLO	0xEE
#define	BROADCAST_ACK	0x55
#define	BROADCAST_ADDR	0x3C

// Enum
enum{
PIPE_0=0,
PIPE_1,
PIPE_2,
PIPE_3,
PIPE_4,
PIPE_5
};

// Possible values: [0,125] (Check Legal ISM bands in your country)
#define RF_CHANNEL	20

// Packet Size
#define PACKET_SIZE	12

// Possible values: 3,4 or 5
#define ADDR_SIZE	5

// Address for Pipe0
#define	ADDR_PIPE0_1	0xDE
#define	ADDR_PIPE0_2	0xAD
#define	ADDR_PIPE0_3	0xBE
#define	ADDR_PIPE0_4	0xEF
#define	ADDR_PIPE0_5	0x00

// Address for all other pipes
#define ADDR_1	0xAB
#define	ADDR_2	0xAB
#define ADDR_3	0x55
#define ADDR_4	0x55

#define ADDR_PIPE1	0x00
#define ADDR_PIPE2	0x33
#define ADDR_PIPE3	0x66
#define ADDR_PIPE4	0x99
#define ADDR_PIPE5	0xCC

//-----------------------------------------------------
// Declaring Addressess
//-----------------------------------------------------
static const char addr_pipe0[ADDR_SIZE] = {ADDR_PIPE0_1,ADDR_PIPE0_2,ADDR_PIPE0_3,ADDR_PIPE0_4,ADDR_PIPE0_5};
static const char addr_pipe1[ADDR_SIZE] = {ADDR_1,ADDR_2,ADDR_3,ADDR_4,ADDR_PIPE1};
static const char addr_pipe2[ADDR_SIZE] = {ADDR_1,ADDR_2,ADDR_3,ADDR_4,ADDR_PIPE2};
static const char addr_pipe3[ADDR_SIZE] = {ADDR_1,ADDR_2,ADDR_3,ADDR_4,ADDR_PIPE3};
static const char addr_pipe4[ADDR_SIZE] = {ADDR_1,ADDR_2,ADDR_3,ADDR_4,ADDR_PIPE4};
static const char addr_pipe5[ADDR_SIZE] = {ADDR_1,ADDR_2,ADDR_3,ADDR_4,ADDR_PIPE5};

//-----------------------------------------------------
// Variable that will store what is read from the antenna
//-----------------------------------------------------
#if defined CENTRAL_NODE
	static char RXBuffer[3][PACKET_SIZE] = {0};
#else
	#if defined ON_OFF_SWITCH
	static char RXBuffer[PACKET_SIZE] = {0};
	#endif
#endif

unsigned int ListenPipe(unsigned int pipe);

#endif /*HOME_AUTOMATION*/
#endif /* HOMEAUTOMATION_H_ */
