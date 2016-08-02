#include "msp430g2553.h"
#include <stdint.h>

#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Transmitter\I2C_MSP430.c"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Transmitter\MSP430_MPU6050.c"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Transmitter\msprf24.c"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Transmitter\nrf_userconfig.h"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Transmitter\interruptions_vectors.c"


//*****************************************************
//Control LED defines
//*****************************************************
#define LEDTX			BIT3
#define LEDTXON			P2OUT |= BIT3;
#define LEDTXOFF		P2OUT &=~BIT3;
#define LEDTXFAILED		BIT4
#define LEDTXFAILEDON	P2OUT |= BIT4;
#define LEDTXFAILEDOFF	P2OUT &=~BIT4;
#define LEDRX			BIT5
#define LEDRXON			P2OUT |= BIT5;
#define LEDRXOFF		P2OUT &=~BIT5;


//*****************************************************
//I2C SCL e SDA
//*****************************************************
#define SCL				BIT6
#define SDA				BIT7


//*****************************************************
//Define the size of the received vector
//*****************************************************
#define vector_size		12
unsigned char send[vector_size] = {
//		 00  01  02
		'X','D','D',
//		 03  04  05
		'Y','D','D',
//		 06  07  08
		'Z','D','D',
//		 09  10  11
		'T','D','D'
};


int main(void)
{

	//*****************************************************
	//nRF addr ;
	//*****************************************************
	char addr[5];


	 WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
	 BCSCTL1 = CALBC1_1MHZ;
	 DCOCTL = CALDCO_1MHZ;

	 //*****************************************************
	 //Inputs & Outputs
	 //*****************************************************
	 P1DIR |= BIT0+BIT3;
	 P1SEL |= SCL + SDA;
	 P1SEL2|= SCL + SDA;
	 P2DIR |= LEDTX+LEDTXFAILED+LEDRX;
	 P2OUT &= ~(LEDTX+LEDTXFAILED+LEDRX);


	 //*****************************************************
	 //Defines variables for the nRF library
	 //*****************************************************
	 rf_crc = RF24_EN_CRC | RF24_CRCO; // CRC enabled, 16-bit
	 rf_addr_width      = 5;
	 rf_speed_power     = RF24_SPEED_1MBPS | RF24_POWER_0DBM;
     rf_channel         = 120;


     //*****************************************************
     //Initialize MPU: internal clock, gyro full scale = 1000, accelerometer full scale = 2g
     //*****************************************************
	 initializeIMU(MPU6050_CLOCK_INTERNAL,MPU6050_GYRO_FS_1000 ,MPU6050_ACCEL_FS_2 );
	 //*****************************************************
	 //Set Sample Rate as minimum
	 //*****************************************************
	 sampleRate(0xFF);


	 //*****************************************************
	 //initialize nFR ; configure and open pipes ;
	 //*****************************************************
	 msprf24_init();
	 msprf24_set_pipe_packetsize(0,vector_size);
	 msprf24_open_pipe(0,1);  // Open pipe#0 with Enhanced ShockBurst enabled


	 //*****************************************************
	 //Receiving and transmitting address (must be equal to the Receiver)
	 //*****************************************************
	 msprf24_standby();
	 addr[0] = 0xDE;	addr[1] = 0xAD;	addr[2] = 0xBE;	addr[3] = 0xEF;	addr[4] = 0x00;
	 w_tx_addr(addr);
	 w_rx_addr(0, addr);  // Pipe 0 receives auto-ack's, autoacks are sent back to the TX addr so the PTX node
			             // needs to listen to the TX addr on pipe#0 to receive them.


	 //*****************************************************
	 //Main loop
	 //*****************************************************
	 while(1){

		 //*****************************************************
		 //Flush TX to ensure there's nothing left from previous usage
		 //*****************************************************
		 flush_tx();


		 //*****************************************************
		 //Load TX register with the "send" vector ; put MSP to sleep until IRQ is low
		 //*****************************************************
		 w_tx_payload(vector_size,send);
		 msprf24_activate_tx();
		 LPM4;


		 //*****************************************************
		 //Check if TX or TXFAILED or RX flags are up
		 //*****************************************************
		 if (rf_irq & RF24_IRQ_FLAGGED) {
			msprf24_get_irq_reason();

			if (rf_irq & RF24_IRQ_TX) 		LEDTXON;

			if (rf_irq & RF24_IRQ_TXFAILED)	LEDTXFAILEDON;

			if (rf_irq & RF24_IRQ_RX) 		LEDRXON;


			//*****************************************************
			//Clear IRQ mask
			//*****************************************************
			msprf24_irq_clear(RF24_IRQ_MASK);


			//*****************************************************
			//Arbitrary delay
			//*****************************************************
			__delay_cycles(100000);


			//*****************************************************
			//Set all LED off
			//*****************************************************
			LEDTXOFF;LEDTXFAILEDOFF;LEDRXOFF;
			}


		 //*****************************************************
		 //Get data from MPU-6050
		 //*****************************************************
		 get_accel_temp_separated(&send[1],&send[2],&send[4],&send[5],&send[7],&send[8],&send[10],&send[11]);
	 }
}
