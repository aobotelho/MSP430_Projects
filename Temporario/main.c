//******************************************************************************
//  MSP430G2xx3 Demo - USCI_B0 I2C Master TX/RX multiple bytes from MSP430 Slave
//                     with a repeated start in between TX and RX operations.
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  transmits to the slave, then a repeated start is generated followed by a
//  receive operation. This is the master code. This code demonstrates how to
//  implement an I2C repeated start with the USCI module using the USCI_B0 TX
//  interrupt.
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.2MHz
//
//	***to be used with msp430x22x4_uscib0_i2c_13.c***
//
//                                /|\  /|\
//               MSP430F24x      10k  10k     MSP430G2xx3
//                   slave         |    |        master
//             -----------------   |    |  -----------------
//           -|XIN  P3.1/UCB0SDA|<-|---+->|P3.1/UCB0SDA  XIN|-
//            |                 |  |      |                 |
//           -|XOUT             |  |      |             XOUT|-
//            |     P3.2/UCB0SCL|<-+----->|P3.2/UCB0SCL     |
//            |                 |         |                 |
//
//  D. Dang
//  Texas Instruments Inc.
//  February 2011
//  Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************



#include <msp430.h>
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\I2C\functions.c"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\GY-52\registers.c"

#define NUM_BYTES_TX 1                         // How many bytes?
#define NUM_BYTES_RX 2

int RXByteCtr, RPT_Flag = 0;                // enables repeated start when 1
volatile unsigned char RxBuffer[128];       // Allocate 128 byte of RAM
unsigned char *PTxData;                     // Pointer to TX data
unsigned char *PRxData;                     // Pointer to RX data
unsigned char TXByteCtr, RX = 0;
unsigned char MSData = 0x55;

//***********************************************************************************************

unsigned int tx_count = 0;

//***********************************************************************************************

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  //MCLK = 1MHz
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;

  P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0

  while(1){

  //Transmit process
  Setup_I2C_TX(&RX);
  RPT_Flag = 1;


  //sample rate divider = 1
  Transmit_I2C(0x00,1,PTxData,TXByteCtr,SMPRT_DIV);
  while (UCB0CTL1 & UCTXSTP);
  //Enable Temperature to be written on the FIFO register.
  Transmit_I2C(0xC0,1,PTxData,TXByteCtr,FIFO_EN);
  while (UCB0CTL1 & UCTXSTP);
  //power management = 0
  Transmit_I2C(0x00,1,PTxData,TXByteCtr,PWR_MGMT_1);
  while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent

  //Receive process
  Setup_I2C_RX(&RX);
  Receive_I2C(RxBuffer,1,PRxData,RXByteCtr,WHO_AM_I);
  while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent

  Setup_UART();

  }
}

//-------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
// any 2+ number of bytes by pre-loading RXByteCtr with the byte count.
//-------------------------------------------------------------------------------
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
  if(RX == 1){                              // Master Recieve?
  RXByteCtr--;                              // Decrement RX byte counter
  if (RXByteCtr)
  {
    *PRxData++ = UCB0RXBUF;                 // Move RX data to address PRxData
  }
  else
  {
    if(RPT_Flag == 0)
        UCB0CTL1 |= UCTXSTP;                // No Repeated Start: stop condition
      if(RPT_Flag == 1){                    // if Repeated Start: do nothing
        RPT_Flag = 0;
      }
    *PRxData = UCB0RXBUF;                   // Move final RX data to PRxData
    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
  }}

  else{                                     // Master Transmit
      if (TXByteCtr)                        // Check TX byte counter
  {
    UCB0TXBUF = MSData++;                   // Load TX buffer
    TXByteCtr--;                            // Decrement TX byte counter
  }
  else
  {
    if(RPT_Flag == 1){
    RPT_Flag = 0;
    PTxData = &MSData;                      // TX array start address
    TXByteCtr = NUM_BYTES_TX;                  // Load TX byte counter
    __bic_SR_register_on_exit(CPUOFF);
    }
    else{
    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
    }
  }
 }

}


#pragma vector=USCIA0TX_VECTOR
__interrupt void UART_interrupt(void){
    if(tx_count<=(7))	UCA0TXBUF = RxBuffer[ tx_count++ ];
    else{
    	IE2 &= ~UCA0TXIE;
    	tx_count=0;
    	__bic_SR_register_on_exit(CPUOFF);
    }

}


