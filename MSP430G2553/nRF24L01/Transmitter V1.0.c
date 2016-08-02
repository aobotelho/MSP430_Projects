#include "msp430g2553.h"
#include <stdint.h>

// Include I2C MSP430 and MPU6050 libraries.
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Transmitter\I2C_MSP430.c"						// Use the correct I2C library particular to your MCU.
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Transmitter\MSP430_MPU6050.c"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Transmitter\interruptions_vectors.c"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Transmitter\msprf24.c"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Transmitter\nrf_userconfig.h"

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-Andr�_Beginning_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-


#define LEDTX			BIT3
#define LEDTXON			P2OUT |= BIT3;
#define LEDTXOFF		P2OUT &=~BIT3;
#define LEDTXFAILED		BIT4
#define LEDTXFAILEDON	P2OUT |= BIT4;
#define LEDTXFAILEDOFF	P2OUT &=~BIT4;
#define LEDRX			BIT5
#define LEDRXON			P2OUT |= BIT5;
#define LEDRXOFF		P2OUT &=~BIT5;

#define SCL				BIT6
#define SDA				BIT7

#define vector_size		12

char send[vector_size];

volatile unsigned int user;

char addr[5];
char buf[32];

unsigned int is_open;
unsigned int is_alive;

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-Andr�_End_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
int main(void)
{

	int i;

	 WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
	 BCSCTL1 = CALBC1_1MHZ;
	 DCOCTL = CALDCO_1MHZ;

	 P1DIR |= BIT0+BIT3;
	 P1SEL |= SCL + SDA;
	 P1SEL2|= SCL + SDA;
	 P2DIR |= LEDTX+LEDTXFAILED+LEDRX;
	 P2OUT &= ~(LEDTX+LEDTXFAILED+LEDRX);

	 i=0;
	 send[i++]='D'; //0
	 send[i++]='E'; //1
	 send[i++]='V'; //2
	 send[i++]='I'; //3
	 send[i++]='C'; //4
	 send[i++]='E'; //5
	 send[i++]=' '; //6
	 send[i++]='I'; //7
	 send[i++]='D'; //8
	 send[i++]=':'; //9
	 send[i++]=' '; //10
	 send[i++]=':'; //11

	 getDeviceID(&send[11]);



	 rf_crc = RF24_EN_CRC | RF24_CRCO; // CRC enabled, 16-bit
	 rf_addr_width      = 5;
	 rf_speed_power     = RF24_SPEED_1MBPS | RF24_POWER_0DBM;
     rf_channel         = 120;


     //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-Andr�_beginning_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
	 initializeIMU(MPU6050_CLOCK_INTERNAL,MPU6050_GYRO_FS_1000 ,MPU6050_ACCEL_FS_2 );						// Initialize IMU
	 sampleRate(0xFF);
	 //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-Andr�_End_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-


	 msprf24_init();  // All RX pipes closed by default
	 msprf24_set_pipe_packetsize(0,vector_size);
	 msprf24_open_pipe(0,1);  // Open pipe#0 with Enhanced ShockBurst enabled for receiving Auto-ACKs
	 // Transmit to 'rad01' (0x72 0x61 0x64 0x30 0x31)
	 msprf24_standby();
	 user = msprf24_current_state();
	 addr[0] = 0xDE;	addr[1] = 0xAD;	addr[2] = 0xBE;	addr[3] = 0xEF;	addr[4] = 0x00;
	 w_tx_addr(addr);
	 w_rx_addr(0, addr);  // Pipe 0 receives auto-ack's, autoacks are sent back to the TX addr so the PTX node
			             // needs to listen to the TX addr on pipe#0 to receive them.

	 //test if nRF is alive
	 is_alive= msprf24_is_alive();
	 //test if the pipe is open
	 is_open = msprf24_pipe_isopen(0);


	 while(1){
		 flush_tx();
		 w_tx_payload(vector_size,send);

		 msprf24_activate_tx();

		 //************
		 //Instead of LPM4 maybe I could use current_state()&RF24_STATE_PRX
		 //************
		 LPM4;

			if (rf_irq & RF24_IRQ_FLAGGED) {
				msprf24_get_irq_reason();
				if (rf_irq & RF24_IRQ_TX){
					LEDTXON;
				}
				if (rf_irq & RF24_IRQ_TXFAILED){
					LEDTXFAILEDON;
				}
				if (rf_irq & RF24_IRQ_RX){
					LEDRXON;
				}

				msprf24_irq_clear(RF24_IRQ_MASK);
				user = msprf24_get_last_retransmits();
				__delay_cycles(100000);
				LEDTXOFF;LEDTXFAILEDOFF;LEDRXOFF;
			}
			 	 	 	 	 	 	  send[0]='X'; 		send[3]='Y';	  send[6]='Z';		send[9]='T';
			get_accel_temp_separated(&send[1],&send[2],&send[4],&send[5],&send[7],&send[8],&send[10],&send[11]);
	 }
}
