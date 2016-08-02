#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <msp430.h>

typedef enum{
	FALSE=0,
	TRUE
}BOOL;

//#define CLOCK_1MHZ
//#define CLOCK_8MHZ
#define CLOCK_16MHZ

//#define USE_CONTROL_LED

//#define USE_UART
//#define USE_I2C
//#define USE_SPI_USCIA
//#define USE_SPI_USCIB

//#define UART_USE_TX_INTERRUPT
//#define UART_USE_RX_INTERRUPT
//#define SPI_USE_TX_INTERRUPT
//#define SPI_USE_RX_INTERRUPT
//#define I2C_USE_TX_INTERRUPT
//#define I2C_USE_RX_INTERRUPT

//#define USE_NRF24L01

//#define USE_SERIAL_LCD

#define USE_TIMERA

//#define HOME_AUTOMATION


#endif /* DEFINITIONS_H_ */
