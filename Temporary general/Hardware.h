#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "Definitions.h"

#if defined USE_CONTROL_LED
	#define LED0_IO		BIT5
	#define LED0_PORT	P2OUT
	#define LED0_DIR	P2DIR
	#define	LED0_ON		LED0_PORT |= LED0_IO
	#define LED0_OFF	LED0_PORT &= ~LED0_IO
	#define LED1_IO		BIT4
	#define LED1_PORT	P2OUT
	#define LED1_DIR	P2DIR
	#define	LED1_ON		LED1_PORT |= LED1_IO
	#define LED1_OFF	LED1_PORT &= ~ LED1_IO
	#define LED2_IO		BIT3
	#define LED2_PORT	P2OUT
	#define LED2_DIR	P2DIR
	#define	LED2_ON		LED2_PORT |= LED2_IO
	#define LED2_OFF	LED2_PORT &= ~ LED2_IO
	#define LED3_IO		BIT3
	#define LED3_PORT	P1OUT
	#define LED3_DIR	P1DIR
	#define	LED3_ON		LED3_PORT |= LED3_IO
	#define LED3_OFF	LED3_PORT &= ~ LED3_IO
	#define LED4_IO		BIT2
	#define LED4_PORT	P1OUT
	#define LED4_DIR	P1DIR
	#define	LED4_ON		LED4_PORT |= LED4_IO
	#define LED4_OFF	LED4_PORT &= ~ LED4_IO
	#define LED5_IO		BIT1
	#define LED5_PORT	P1OUT
	#define LED5_DIR	P1DIR
	#define	LED5_ON		LED5_PORT |= LED5_IO
	#define LED5_OFF	LED5_PORT &= ~ LED5_IO
	#define LED6_IO		BIT0
	#define LED6_PORT	P1OUT
	#define LED6_DIR	P1DIR
	#define	LED6_ON		LED6_PORT |= LED6_IO
	#define LED6_OFF	LED6_PORT &= ~LED6_IO

	#if defined USE_NRF24L01
		#define LEDTX_IO			LED0_IO
		#define LEDTX_PORT			LED0_PORT
		#define LEDTX_ON			LED0_ON
		#define LEDTX_OFF			LED0_OFF
		#define LEDTXFAILED_IO		LED1_IO
		#define LEDTXFAILED_PORT	LED1_PORT
		#define LEDTXFAILED_ON		LED1_ON
		#define LEDTXFAILED_OFF		LED1_OFF
		#define LEDRX_IO			LED2_IO
		#define LEDRX_PORT			LED2_PORT
		#define LEDRX_ON			LED2_ON
		#define LEDRX_OFF			LED2_OFF
	#endif
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
			#define MOSI	BIT2
			#define MISO	BIT1
			#define SCK	BIT4
			#define SS		BIT5
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
			#define MOSI	BIT7
			#define MISO	BIT6
			#define SCK		BIT5
			#define SS		BIT4
		#endif
	#endif
#endif

#if defined USE_NRF24L01
	#include "msprf24.h"
#endif

#if defined HOME_AUTOMATION
	#include "HomeAutomation.h"
#endif

#endif /* HARDWARE_H_ */
