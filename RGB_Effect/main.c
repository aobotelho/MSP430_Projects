#include <msp430.h>

enum{
	RED = 0,
	GREEN,
	BLUE,
	ALL
};

#define RED_ADC_BIT		BIT1
#define	GREEN_ADC_BIT	BIT2
#define	BLUE_ADC_BIT	BIT3
#define HIGHEST_PORT	INCH_3

#define	LED_BIT			BIT6

#define	COUNT_MAX		0x2000

#define RED_TRAN_BIT	BIT0
#define	GREEN_TRAN_BIT	BIT1
#define	BLUE_TRAN_BIT	BIT2

unsigned int CurrentLED = RED;

unsigned int ResetFlag = 0;

unsigned int Counter = 0;
unsigned int up = 1;

int main( void ){
	WDTCTL = WDTPW + WDTHOLD; // Disable watchdog timer
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;

	// LED Out
	P1DIR |= LED_BIT;
	// PWM Out
	P2DIR |= RED_TRAN_BIT | GREEN_TRAN_BIT | BLUE_TRAN_BIT;

	//******************************************************
	//	TACTL: see page 370 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
	//		- TASSEL_ : Clock Source
	//			+ TASSEL_2: SMCLK
	//		- ID_ : Input Divider
	//			+ ID_0: /1
	//		- MC_ : Mode Control
	//			+ MC_1: Up mode (Up to TACCR0)
	//******************************************************
	TACTL = TASSEL_2 | ID_0 | MC_1 ;

	//******************************************************
	//	TACCTL0:see page 372 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
	//		- CM_ : Capture Mode
	//			+ CM_0: No Capture
	//		- CCIE: Enable Capture/Compare interrupt
	//******************************************************
	TACCR0 = COUNT_MAX;
	TACCTL0 = CM_0 | CCIE ;

	//******************************************************
	//	TACCTL1:see page 372 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
	//		- CM_ : Capture Mode
	//			+ CM_0: No Capture
	//		- CCIE: Enable Capture/Compare interrupt
	//******************************************************
	TA0CCR1 = 1000;
	TA0CCTL1 = CM_0 | CCIE;

	_BIS_SR(GIE);


	while(1);

}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void){

	if(CurrentLED != ALL)	P2OUT |= (1<<CurrentLED);
	else P2OUT |= (RED_TRAN_BIT | GREEN_TRAN_BIT | BLUE_TRAN_BIT);
	Counter++;
	if(Counter == COUNT_MAX/2){
		up = 0;
	}
	else if(Counter == COUNT_MAX-50){
		Counter = 10;
		TA0CCR1 = 10;
		up = 1;
		if(++CurrentLED == (ALL+1)) CurrentLED = 0;
	}


	if(up)	TA0CCR1 += 1;
	else TA0CCR1 -= 1;

	TA0CCTL0 &= ~CCIFG;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void){
	P2OUT &= ~(RED_TRAN_BIT | GREEN_TRAN_BIT | BLUE_TRAN_BIT);
	TA0CCTL1 &= ~CCIFG;
}
