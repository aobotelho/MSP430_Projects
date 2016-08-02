#include "msp430g2553.h"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Receiver\msprf24.c"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Receiver\nrf_userconfig.h"
#include "C:\ti\ccsv5\ccs_base\msp430\include\myLibs\Receiver\functions.c"
#include "stdint.h"

//LED
#define LED0			BIT0
#define LED0ON	P1OUT|= LED0
#define	LED0OFF	P1OUT&=~LED0
#define LED1			BIT3
#define LED1ON	P1OUT|= LED1
#define	LED1OFF	P1OUT&=~LED1

#define LEDTX			BIT3
#define LEDTXON			P2OUT |= BIT3;
#define LEDTXOFF		P2OUT &=~BIT3;
#define LEDTXFAILED		BIT4
#define LEDTXFAILEDON	P2OUT |= BIT4;
#define LEDTXFAILEDOFF	P2OUT &=~BIT4;
#define LEDRX			BIT5
#define LEDRXON			P2OUT |= BIT5;
#define LEDRXOFF		P2OUT &=~BIT5;

//UART TX and RX
#define RX			BIT1
#define TX			BIT2


#define vector_size 12
char buf[vector_size];

#define send_size 	33
char send[send_size]={' ','X',':',' ','D','D','D','D',';','Y',':',' ','D','D','D','D',';','Z',':',' ','D','D','D','D',';','T',':',' ','D','D',';','\r',0x0B};

unsigned int tx_count=0;
volatile unsigned int user;

unsigned int temporary[4];
 char temp[4];


void uart_init(){
	//UART configuration
	    	UCA0CTL1 =
	    			UCSWRST;
	    	//Set RX and TX


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
	char addr[5];
	unsigned int i;


	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	//MCLK = 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	//All pins but accelerometers output + Light up P1.0 LED
  	P1DIR |= TX + LED0+LED1;
  	P1DIR &= ~(RX);
  	P1OUT = ~LED0+LED1;
  	P1SEL = TX + RX;
  	P1SEL2 = TX + RX;
  	P2DIR |= LEDTX+LEDTXFAILED+LEDRX;


	// Initial values for nRF24L01+ library config variables
	rf_crc = RF24_EN_CRC | RF24_CRCO; // CRC enabled, 16-bit
	rf_addr_width      = 5;
	rf_speed_power     = RF24_SPEED_1MBPS | RF24_POWER_0DBM;
	rf_channel         = 120;

	msprf24_init();
	msprf24_set_pipe_packetsize(0,vector_size);
	msprf24_open_pipe(0, 1);  // Open pipe#0 with Enhanced ShockBurst
	// Set our RX address
	addr[0] = 0xDE;	addr[1] = 0xAD;	addr[2] = 0xBE;	addr[3] = 0xEF;	addr[4] = 0x00;
	w_rx_addr(0, addr);
	flush_rx();




	while(1)
	{
		msprf24_activate_rx();
		LPM4;



		if (rf_irq & RF24_IRQ_FLAGGED) {
			msprf24_get_irq_reason();
			if (rf_irq & RF24_IRQ_TX){
				LEDTXON;
			}
			if (rf_irq & RF24_IRQ_TXFAILED){
				LEDTXFAILEDON;
			}
		}

		if (rf_irq & RF24_IRQ_RX||msprf24_rx_pending()){
				LEDRXON;
				r_rx_payload(vector_size, buf);


				temporary[0] = (( buf[1]  << 8) | buf[2])*15.2285-999 ;
				temporary[1] = (( buf[4]  << 8) | buf[5])*15.2285-999 ;
				temporary[2] = (( buf[7]  << 8) | buf[8])*15.2285-999 ;
				temporary[3] = ((( buf[10] << 8) | buf[11])+12421)/340;


				itoa(temporary[0],temp,10);
				send[4]=temp[0];
				send[5]=temp[1];
				send[6]=temp[2];
				send[7]=temp[3];
				for(i=0;i<4;i++)temp[i]=0;
				itoa(temporary[1],temp,10);
				send[12]=temp[0];
				send[13]=temp[1];
				send[14]=temp[2];
				send[15]=temp[3];
				for(i=0;i<4;i++)temp[i]=0;
				itoa(temporary[2],temp,10);
				send[20]=temp[0];
				send[21]=temp[1];
				send[22]=temp[2];
				send[23]=temp[3];
				for(i=0;i<4;i++)temp[i]=0;
				itoa(temporary[3],temp,10);
				send[28]=temp[0];
				send[29]=temp[1];
				for(i=0;i<4;i++)temp[i]=0;
		}


			uart_init();
			IE2 = UCA0TXIE;
			__bis_SR_register(CPUOFF + GIE);

			if(msprf24_queue_state()&RF24_QUEUE_RXFULL) flush_rx();
			LEDTXOFF;LEDTXFAILEDOFF;LEDRXOFF;
	}
}





#pragma vector=USCIAB0TX_VECTOR
__interrupt void UART_interrupt(void){
    if(tx_count<=send_size)	UCA0TXBUF = send[ tx_count++ ];
    else{
    	IE2 &= ~UCA0TXIE;
    	tx_count=0;
    	__bic_SR_register_on_exit(CPUOFF);
    }

}

