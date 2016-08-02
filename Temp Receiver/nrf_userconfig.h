#ifndef _NRF_USERCONFIG_H
#define _NRF_USERCONFIG_H

#include "Definitions.h"
/* CPU clock cycles for the specified amounts of time--accurate minimum delays
 * required for reliable operation of the nRF24L01+'s state machine.
 */
#if defined CLOCK_1MHZ
	#if defined CLOCK_8MHZ
		#pragma CLOCK_CONFLICT
	#else
		#if defined CLOCK_16MHZ
			#pragma CLOCK_CONFLICT
		#else
			// No clock conflict
			#define DELAY_CYCLES_5MS       5000
			#define DELAY_CYCLES_1MS	   DELAY_CYCLES_5MS/5
			#define DELAY_CYCLES_130US     130
			#define DELAY_CYCLES_15US      15
		#endif
	#endif
#else
	#if defined CLOCK_8MHZ
		#if defined CLOCK_16MHZ
			#pragma CLOCK_CONFLICT
		#else
			// No clock conflict
			#define DELAY_CYCLES_5MS       40000
			#define DELAY_CYCLES_1MS	   DELAY_CYCLES_5MS/5
			#define DELAY_CYCLES_130US     1040
			#define DELAY_CYCLES_15US      120
		#endif
	#else
		#if defined CLOCK_16MHZ
			// No clock conflict
			#define DELAY_CYCLES_5MS       80000
			#define DELAY_CYCLES_1MS	   DELAY_CYCLES_5MS/5
			#define DELAY_CYCLES_130US     2080
			#define DELAY_CYCLES_15US      240
		#else
			#pragma DEFINE_CLOCK
		#endif
	#endif
#endif

/* SPI port--Select which USCI port we're using.
 * Applies only to USCI devices.  USI users can keep these
 * commented out.
 */
//#define RF24_SPI_DRIVER_USCI_A 1
#define RF24_SPI_DRIVER_USCI_B 1

/* Define whether this library should use LPM0+IRQs during SPI I/O and whether this library should provide the ISR. */
//#define RF24_SPI_DRIVER_USCI_USE_IRQ 1
#define RF24_SPI_DRIVER_USCI_PROVIDE_ISR 1


/* Operational pins -- IRQ, CE, CSN (SPI chip-select)
 */

/* IRQ */
#define nrfIRQport 2
#define nrfIRQpin BIT2

/* CSN SPI chip-select */
#define nrfCSNport 2
#define nrfCSNportout P2OUT
#define nrfCSNpin BIT1

/* CE Chip-Enable (used to put RF transceiver on-air for RX or TX) */
#define nrfCEport 2
#define nrfCEportout P2OUT
#define nrfCEpin BIT0

#endif
