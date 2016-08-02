#ifndef SPICONFIG_H_
#define SPICONFIG_H_

#include "Hardware.h"


#if defined USE_SPI_USCIA
	#define SPIA_BUFFER_SIZE	32

	typedef struct{
		unsigned char SendBuffer[SPIA_BUFFER_SIZE];
		unsigned char ReceiveBuffer[SPIA_BUFFER_SIZE];
		unsigned int SendByteCounter;
		unsigned int ReceiveByteCounter;
		unsigned int FinishSend;
		unsigned int FinishReceive;
	}SPI_VAR;

	SPI_VAR SPIvarA;

	void SPIInit_USCIA(void);

	#if defined USE_NRF24L01
		//Compatibility with Spirilis nRF library
		#define spi_init()	SPIInit_USCIB()
		#define spi_transfer(a)	SPIB_transfer(a)
		#define spi_transfer16(a)	SPIB_transfer16(a)
	#endif
#endif

#if defined USE_SPI_USCIB
	#define SPIB_BUFFER_SIZE	32

	typedef struct{
		unsigned char SendBuffer[SPIB_BUFFER_SIZE];
		unsigned char ReceiveBuffer[SPIB_BUFFER_SIZE];
		unsigned int SendByteCounter;
		unsigned int ReceiveByteCounter;
		unsigned int FinishSend;
		unsigned int FinishReceive;
	}SPI_VAR;

	SPI_VAR SPIvarB;

	void SPIInit_USCIB(void);
	unsigned char SPIB_transfer(char inb);
	unsigned int SPIB_transfer16(int inw);

	#if defined USE_NRF24L01
		//Compatibility with Spirilis nRF library
		#define spi_init()	SPIInit_USCIB()
		#define spi_transfer(a)	SPIB_transfer(a)
		#define spi_transfer16(a)	SPIB_transfer16(a)
	#endif
#endif



#endif /* SPICONFIG_H_ */
