#include "msp430g2553.h"
#include "funcoes.c"

//Time between measurements (ms) - max: 524 @125KHz (1MHz/8)
#define measure_time 100

//Accelerometer axis
#define Xaxis		BIT3
#define Xaxis_adc	INCH_3
#define Yaxis		BIT4
#define Yaxis_adc	INCH_4
#define Zaxis		BIT5
#define Zaxis_adc	INCH_5

//Control LED
#define LED 		BIT0

//UART TX and RX
#define TX			BIT2
#define RX			BIT1


//************************************VARIAVEL AUXILIAR PARA PROGAMAÇÃO******************************************
int axis=1;
int value=0;
char buffer[4];
unsigned int aux=0;
int tx_count = 0;
char send[7];
unsigned int count;
unsigned int limite=6;

void main(void)
{
	send[1] = ':';
	send[2] = ' ';


	//Stop Watchdog
	WDTCTL = WDTPW + WDTHOLD;
	//MCLK = 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;


    //All pins but accelerometers output + Light up P1.0 LED
    P1DIR |= LED + TX + BIT6;
    P1DIR &= ~(RX+Xaxis+Yaxis+Zaxis);
    P1OUT = LED;


//Timer_A configuration
    //Count up to measure_frequency
    TACCR0 = (measure_time*125);
    //SMCLK (TASSEL_2) - we can use LMP0, Up Mode (MC_1),CLK Div /8 (ID_3) - 125kHz, Timer_A Clear (TACLR)
    TACTL = TASSEL_2 + MC_1 + ID_3 + TACLR;
    //TACCR0 interrupt enable
    TACCTL0 = CCIE;

//ADC10 configuration
    //Ref: Vcc&Vss (SREF_0), sample-and-hold 4 clocks (ADC10SHT_0), On (ADC10ON), ADC10 interrupt on (ADC10IE)
    ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON + ADC10IE;
    //CLK div/4 (ADC10DIV_4), SMCLK (ADC10SSEL_3) - we can use LMP0, Single channel single conversion (CONSEQ_0)
    ADC10CTL1 = ADC10DIV_4 + ADC10SSEL_3 + CONSEQ_0;
    //Enable pins P1.3 & P1.4 & P1.5 as analog input
    ADC10AE0 = Xaxis + Yaxis + Zaxis;


//UART configuration
    UCA0CTL1 = UCSWRST;
    //Set RX and TX
    P1SEL = TX + RX;
    P1SEL2 = TX + RX;

    //Set register to a Baud Rate of 9600 (10^6/9600 = 104)
    UCA0BR0 = 104;
    UCA0BR1 = 0x00;
    //Second stage modulation 1 (UCBRS_1)
    UCA0MCTL = UCBRS0;
    //SMCLK (UCSSEL_2), Start USCI (&=~UCSWRST)
    UCA0CTL1 = UCSSEL_2;
    UCA0CTL1  &= ~UCSWRST;


    //Enable Interruptions
    __enable_interrupt();

    while( 1 ){
    	//Set CPU on Low Power Mode
    	_BIS_SR(LPM0_bits);
    }
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0_interrupt(void){
	if(axis==1) {ADC10CTL1 = Xaxis_adc; send[0] = 'x';}
	if(axis==2) {ADC10CTL1 = Yaxis_adc; send [0] = 'y';}
	if(axis==3) {ADC10CTL1 = Zaxis_adc; axis=0; send[0] = 'z';}
	axis++;
	ADC10CTL0 |= ADC10SC+ENC;
	//Wait for the conversion to be done
	while( ADC10CTL1 & ADC10BUSY ) ;
	P1OUT ^= BIT0;
	value = ADC10MEM;
	for(count=4;count>0;count--){
		buffer[count-1]=' ';
	}
	itoa(ADC10MEM,buffer,10);
	for(count=4;count>0;count--){
		send[count+2]=buffer[count-1];
	}
	for(count=7;count>0;count--){
		if(send[count-2]== ' '||send[count-2]=='.') limite--;
	}
    //Enable TX interrupt (UCA0TXIE)
	ADC10CTL0 &= ~ENC;
	IE2 = UCA0TXIE;

}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_interrupt(void){
	P1OUT ^= BIT6;
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void UART_interrupt(void){
    if(tx_count<=limite)	UCA0TXBUF = send[ tx_count++ ];
    else if(tx_count==(limite+1)) {UCA0TXBUF = '\r';tx_count++;}
    else if(tx_count==(limite+2)) {UCA0TXBUF = 0x0B;tx_count++; aux=0;}
    else {IE2 &= ~UCA0TXIE;tx_count=0;limite=6;}

}


