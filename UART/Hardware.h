#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "Definitions.h"

#if defined USE_CONTROL_LED
	#define LEDTX			BIT5
	#define LEDTXON			P2OUT |= BIT5;
	#define LEDTXOFF		P2OUT &=~BIT5;
	#define LEDTXFAILED		BIT4
	#define LEDTXFAILEDON	P2OUT |= BIT4;
	#define LEDTXFAILEDOFF	P2OUT &=~BIT4;
	#define LEDRX			BIT3
	#define LEDRXON			P2OUT |= BIT3;
	#define LEDRXOFF		P2OUT &=~BIT3;
#endif


#if defined USE_UART
	#include "UARTConfig.h"
	#define UART_RX			BIT1
	#define UART_TX			BIT2
#endif

#if defined USE_I2C
	#define SDA	BIT7
	#define	SCL	BIT6
#endif

#if defined USE_SPI_USCIA

	#if defined	USE_UART
		#pragma You_cant_use_SPI_on_USCIA_and_UART
	#else
		#if defined USE_SPI_USCIB
			#pragma You_Cant_Use_SPI_ON_BOTH_USCI
		#else
			#include "SPIConfig.h"
			#define MOSIA	BIT2
			#define MISOA	BIT1
			#define SCKA	BIT4
			#define SCA		BIT5
		#endif
	#endif
#endif

#if defined USE_SPI_USCIB

	#if defined USE_I2C
		#pragma You_cant_use_SPI_on_USCIB_and_I2C
	#else
		#if defined USE_SPI_USCIA
			#pragma You_Cant_Use_SPI_ON_BOTH_USCI
		#else
			#include "SPIConfig.h"
			#define MOSIB	BIT7
			#define MISOB	BIT6
			#define SCKB	BIT5
			#define SCB		BIT4
		#endif
	#endif
#endif

#endif /* HARDWARE_H_ */
