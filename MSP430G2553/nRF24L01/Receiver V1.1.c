#include "msp430g2553.h"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Receiver\msprf24.c"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Receiver\nrf_userconfig.h"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Receiver\functions.c"
#include "stdint.h"


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
//UART TX and RX
//*****************************************************
#define RX			BIT1
#define TX			BIT2


//*****************************************************
//Define the size of the received vector
//*****************************************************
#define vector_size 12
char buf[vector_size];


//*****************************************************
//Define the size of the vector to send to the PC
//*****************************************************
#define send_size 	33
//					   0   1   2   3   4   5   6   7   8
char send[send_size]={' ','X',':',' ','D','D','D','D',';',

//			               9   10  11  12  13  14  15  16
					      'Y',':',' ','D','D','D','D',';',

//					       17  18  19  20  21  22  23  24
					      'Z',':',' ','D','D','D','D',';',

//						   25  26  27  28  29  30  31  32
					      'T',':',' ','D','D',';','\r',0x0B};


//*****************************************************
//Define initial positions of the axis
//*****************************************************
#define x_pos	4
#define y_pos	12
#define z_pos	20
#define t_pos	28


//*****************************************************
//Auxiliary counter
//*****************************************************
unsigned int tx_count=0;


//*****************************************************
//Define the size of the data to send
//*****************************************************
#define temp_size 4


//*****************************************************
//Define how many axis there'll be
//*****************************************************
#define axis_size 4



#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Receiver\interruptions_vectors.c"



void uart_init(){
	//UART configuration
	    	UCA0CTL1 =
	    			UCSWRST;
	    	//Set register to a Baud Rate of 9600 (10^6/9600 = 104)
	    	UCA0BR0 = 104;
	    	UCA0BR1 = 0x00;
	    	//Second stage modulation 1 (UCBRS_1)
	    	UCA0MCTL = UCBRS0;
	    	//SMCLK (UCSSEL_2), Start USCI (&=~UCSWRST)
	    	UCA0CTL1 = UCSSEL_2;
	    	UCA0CTL1  &= ~UCSWRST;
}



int main(void)
{

	//*****************************************************
	//nRF addr ; auxiliary counter ;
	//*****************************************************
	char addr[5];
	unsigned int i;


	//*****************************************************
	//Temporary variables
	//*****************************************************
	int16_t temporary[axis_size];
	char temp[temp_size];


	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	//MCLK = 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;


	//*****************************************************
	//Inputs & Outputs
	//*****************************************************
  	P1DIR |= TX;
  	P1DIR &= ~(RX);
  	P1SEL = TX + RX;
  	P1SEL2 = TX + RX;
  	P2DIR |= LEDTX+LEDTXFAILED+LEDRX;


  	//*****************************************************
  	//Defines variables for the nRF library
  	//*****************************************************
	rf_crc = RF24_EN_CRC | RF24_CRCO; 							// CRC enabled, 16-bit
	rf_addr_width      = 5;										//Address width = 5
	rf_speed_power     = RF24_SPEED_1MBPS | RF24_POWER_0DBM;	//Speed = 1 MBps ; Power = 0dBm
	rf_channel         = 120;									//Channel = 120


	//*****************************************************
	//initialize nFR ; configure and open pipes ;
	//*****************************************************
	msprf24_init();
	msprf24_set_pipe_packetsize(0,vector_size);
	msprf24_open_pipe(0, 1);  // Open pipe#0 with Enhanced ShockBurst


	//*****************************************************
	//Receiver address (must be equal to the transmitter)
	//*****************************************************
	addr[0] = 0xDE;	addr[1] = 0xAD;	addr[2] = 0xBE;	addr[3] = 0xEF;	addr[4] = 0x00;
	w_rx_addr(0, addr);


	//*****************************************************
	//Flush RX to ensure there's nothing left from previous usage
	//*****************************************************
	flush_rx();

	//*****************************************************
	//Main loop
	//*****************************************************
	while(1)
	{
		//*****************************************************
		//Initiate RX and sleep ; wait until IRQ is low
		//*****************************************************
		msprf24_activate_rx();
		LPM4;


		//*****************************************************
		//Check if TX or TXFAILED flags are up
		//*****************************************************
		if (rf_irq & RF24_IRQ_FLAGGED) {

			msprf24_get_irq_reason();

			if (rf_irq & RF24_IRQ_TX) 		LEDTXON;

			if (rf_irq & RF24_IRQ_TXFAILED)	LEDTXFAILEDON;

			if (rf_irq & RF24_IRQ_RX)	LEDRXON;

			msprf24_irq_clear(RF24_IRQ_MASK);
		}


		//*****************************************************
		//If IRQ RX flag is up enter main loop
		//*****************************************************
		if (msprf24_rx_pending()){

				//*****************************************************
				//Store the RX data on the buf variable
				//*****************************************************
				r_rx_payload(vector_size, buf);


				//*****************************************************
				//Start data manipulation
				//*****************************************************


				//x axis
				//Check if it's in 2's complement format
				if(buf[1]&0x80) buf[1] = ~buf[1]+1;
				temporary[0] = (buf[1] << 8 | buf[2])*0.031-64.934;

				//y axis
				//Check if it's in 2's complement format
				if(buf[4]&0x80) buf[4] = ~buf[4]+1;
				temporary[1] = (buf[4] << 8 | buf[5])*0.0451+100.38;

				//z axis
				//Check if it's in 2's complement format
				if(buf[7]&0x80) buf[7] = ~buf[7]+1;
				temporary[2] = (buf[7] << 8 | buf[8])*0.0389-435.49;

				//temperature axis
				temporary[3] = ((( buf[10] << 8) | buf[11])+12421)/340;

				//x axis
				itoa(temporary[0],temp,10);
				//Store on the "send" vector
				for(i=0;i<temp_size;i++){
					send[x_pos+i] = temp[i];
					temp[i]=0;
				}


				//y axis
				itoa(temporary[1],temp,10);
				//Store on the "send" vector
				for(i=0;i<temp_size;i++){
					send[y_pos+i] = temp[i];
					temp[i]=0;
				}


				//z axis
				itoa(temporary[2],temp,10);
				//Store on the "send" vector
				for(i=0;i<temp_size;i++){
					send[z_pos+i] = temp[i];
					temp[i]=0;
				}


				//temp axis
				itoa(temporary[3],temp,10);
				//Store on the "send" vector
				//Temperature data size = 2
				for(i=0;i<2;i++){
					send[t_pos+i] = temp[i];
					temp[i]=0;
				}
		}

			//*****************************************************
			//Initialize UART configuration to send the data to the PC
			//*****************************************************
			uart_init();


			//*****************************************************
			//Set the interruption ; put MSP to sleep
			//*****************************************************
			IE2 = UCA0TXIE;
			__bis_SR_register(CPUOFF + GIE);


			//*****************************************************
			//Check if RX register is full ; if it is, flush
			//*****************************************************
			if(msprf24_queue_state()&RF24_QUEUE_RXFULL) flush_rx();


			//*****************************************************
			//Set all LED off
			//*****************************************************
			LEDTXOFF;LEDTXFAILEDOFF;LEDRXOFF;
	}
}

