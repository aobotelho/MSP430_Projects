//******************************************************************************************
// RGB LED Output controlled by the value on three potentiometers
//  ________
// |	P2.0|-> Red_Pin
// |	P2.1|-> Green_Pin
// |	P2.2|-> Blue_Pin
// |	P1.3|<- POT1_Red
// |	P1.2|<- POT2_Green
// |    P1.1|<- POT3_Blue
// |    P1.0|<- Button
// |________|
//
// Code Description:
//	First of all a calibration is made. All the pots must be put in the lowest value, then on the highest
// 	This calibration allows a better performance of the counter (PWM)
//
//	Since there is only 2 PWM Outputs a "Fake" PWM is made using Timer A0 and switching the outputs
//******************************************************************************************

#include <msp430.h>

// Enum to help the coding
enum{
	RED = 0,
	GREEN,
	BLUE
};

//Definition of inputs
#define RED_ADC_BIT		BIT1
#define	GREEN_ADC_BIT	BIT2
#define	BLUE_ADC_BIT	BIT3
#define HIGHEST_PORT	INCH_3

#define	LED_BIT			BIT6

#define RED_TRAN_BIT	BIT0
#define	GREEN_TRAN_BIT	BIT1
#define	BLUE_TRAN_BIT	BIT2

// MAX Counter for Timer A0
#define	COUNT_MAX		1000

// ADC Variables
unsigned int ADCResults[3] = {0,0,0};
unsigned int Min[3] = {94,132,131};
unsigned int Max[3] = {917,917,912};

// Current LED Control Variable
unsigned int CurrentLED = RED;

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


	//******************************************************
	//	ADC10CTL0,: see page 553 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
	//		- SREF_ : Voltage Reference
 	//			+ SREF_0: VR+ = AVCC and VR- = AVSS
	//		- ADC10SHT_ : ADC10 sample-and-hold time
 	//			+ ADC10SHT_2: 16 x ADC10CLKs
	//		- ADC10SR: ADC10 Sampling Rate
	//			+ If not set: Reference buffer supports up to ~200kbps
	//		- MSC: Multiple sample and conversion. (Valid only for sequence or repeated modes)
	//			+ If set: The sampling requires a rising edge of the SHI signal to trigger each sample-and-conversion.
	//		- ADC10ON: ADC10 on
	//			+ If set: ADC10 on
	//		- ADC10IE: ADC10 interrupt enable
	//			+ If set: Interrupt enabled
	//******************************************************
	ADC10CTL0 = MSC | ADC10SHT_2 | ADC10ON ;

	//******************************************************
	//	ADC10CTL1:see page 555 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
	//		- INCH_ : Input channel select. (See device specific data sheet)
	//			+ INCH_2: A2 (Highest channel)
	//		- SHS_ : Sample-and-hold source select.
	//			+ SHS_0: ADC10SC bit
	//		- ADC10DF : ADC10 data format
	//			+ If not set: Straight binary
	//		- ISSH: Invert signal sample-and-hold
	//			+ If not set: The sample-input signal is not inverted.
	//		- ADC10DIV_: ADC10 clock divider
	//			+ ADC10DIV_0: /1
	//		- ADC10SSEL_: Clock Source
	//			ADC10SSEL_0: ADC10OSC
	//		- CONSEQ_: Conversion mode select
	//			+ CONSEQ_1: Sequence of channels
	//******************************************************
	ADC10CTL1 = HIGHEST_PORT | CONSEQ_1;

	//******************************************************
	//	ADC10AE0:see page 556 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
	//		- ADC10AE0 : ADC10 analog enable. These bits enable the corresponding pin for analog input. BIT0 corresponds to A0,	BIT1 corresponds to A1, etc.
	//******************************************************
	ADC10AE0 |= RED_ADC_BIT | GREEN_ADC_BIT | BLUE_ADC_BIT ;

	//******************************************************
	//	ADC10DTC1:Data Transfer Control Register 1
	//		- DTC transfers. These bits define the number of transfers in each block.
	//			+ 01h-0FFh - Number of transfers per block
	//******************************************************
	ADC10DTC1 = 0x03;

	//******************************************************
	//Calibration:
	// Put all potentiometers in the lowest value possible and press the button
	// Put all potentiometers in the highest value possible and press the button
	//******************************************************
	/*P1OUT |= LED_BIT;
	while(P1IN & BUTTON_BIT);
	while(!(P1IN & BUTTON_BIT));

	ADC10CTL0 &= ~ENC;
	ADC10SA = (unsigned int)&Min[0]; // Data buffer start
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active

	P1OUT &= ~LED_BIT;
	while(P1IN & BUTTON_BIT);
	while(!(P1IN & BUTTON_BIT));
	P1OUT |= LED_BIT;

	ADC10CTL0 &= ~ENC;
	ADC10SA = (unsigned int)&Max[0]; // Data buffer start
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
	 */

	// Enable Interrupts
	_BIS_SR(GIE);

	//******************************************************
	// Main Loop:
	//	Keeps making ADC conversions
	//	Interrupts are not used so they don't interrupt the timers
	//******************************************************
	while(1){
		// Disable Conversion
		ADC10CTL0 &= ~ENC;
		// Data buffer start
		ADC10SA = (unsigned int)&ADCResults[0];
		// Sampling and conversion start
		ADC10CTL0 |= ENC + ADC10SC;
		// Wait if ADC10 core is active
		while (ADC10CTL1 & BUSY);
	}

}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void){
	// Define flags
	unsigned int FoundPinFlag = 0;
	unsigned int TempCounter = 0;

	// Reset values
	FoundPinFlag = 0;
	TempCounter = 0;

	// Check if pins are above a minimum value
	do{
		// If the ADC result is below a value skip to next pin
		if(ADCResults[CurrentLED] < Min[CurrentLED] + 30){
			if(++CurrentLED > BLUE){
				CurrentLED = RED;
			}
		}
		// If ADC Result is above threshold set a flag
		else{
			FoundPinFlag = 1;
		}
		// Increment temporary counter
		TempCounter++;
	}while(TempCounter < 3 && !FoundPinFlag);

	// If the counter reached a max decrement counter
	// Adds an "arbitrary" delay in the system
	if(TempCounter == 4) CurrentLED--;

	// Specific if outputs are on bits 0-2
	// Only set the output if above a threshold
	if(FoundPinFlag) P2OUT |= (1<<CurrentLED);

	// Set new values for the timer based on the calibration
	TA0CCR1 = ADCResults[CurrentLED];
	TA0CCR0 = Max[CurrentLED] + 50;

	// Resets current value in the counter buffer
	TA0R = 0;

	// Clear interrupt flag
	TA0CCTL0 &= ~CCIFG;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void){
	// Skips to the next pin
	if(++CurrentLED == (BLUE+1)){
		CurrentLED = RED;
	}

	// Reset all pins
	P2OUT &= ~(RED_TRAN_BIT | GREEN_TRAN_BIT | BLUE_TRAN_BIT);

	// Clear interrupt flag
	TA0CCTL1 &= ~CCIFG;
}
