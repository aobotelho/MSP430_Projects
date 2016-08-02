#include "msp430g2553.h"
#include "funcoes.c"

//Time between measurements (ms) - max: 524 @125KHz (1MHz/8)
#define measure_time 50

//Defines the number of conversions to be made by the ADC10
#define number_of_conversions	3

//Variable that receives the conversions values
unsigned int adc[number_of_conversions];

//Highest channel to be converted
#define highest_channel	INCH_5

//Accelerometer axis
#define Xaxis		BIT3
#define Yaxis		BIT4
#define Zaxis		BIT5

//UART TX and RX
#define TX			BIT2
#define RX			BIT1

//Variable used temporarily to convert an integer to a string
unsigned char buffer[4];

//***************************************
unsigned char send[27];
int i;
char teste[10];
unsigned int tx_count=0;


int main(void)
{


  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	//MCLK = 1MHz
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;

  //All pins but accelerometers output + Light up P1.0 LED
  P1DIR |= TX + BIT6;
  P1DIR &= ~(RX+Xaxis+Yaxis+Zaxis);
  P1OUT = BIT6;



//ADC10 configuration
      ADC10CTL0 =
//	    		SREF_0 		// Reference ; See device .h file
    		   	ADC10SHT_2  // 0 - 4 cycles ; 1 - 8 cycles ; 2 - 16 cycles ; 3 - 64 cycles
//	    	  | ADC10SR		// Limits the maximum sampling rate to 50 ksps
//    		  | REFOUT 		// Reference Output
//    		  | REFBURST	// Reference Burst
  		  	  | MSC 		// Multiple sample conversion
//    		  | REFON 		// Reference generator on
//    		  | REF2_5V		// If set: Reference = 2.5V ; if not: Reference = 1.5V
   		  	  | ADC10ON 	// ADC10 on
  		  	  | ADC10IE 	// ADC10 interrupt enable
//    		  | ENC			// Enable conversion
//    		  | ADC10SC		// Start sample-and-conversion
      		  ;

      ADC10CTL1 =
//    			ADC10DIV_4  // N - CLK/(N+1) (e.g. ADC10DIV_4 = CLK/5)
//    		  | ADC10SSEL_0 // CLK source ; 0 - ADC10OSC ; 1 - ACLK ; 2 - MCLK ; 3 - SMCLK
   		  	  	CONSEQ_1	// Conversion mode ; 0 - single channel single conversion
  		    				// 1 - Sequence ; 2 - repeat single ; 3 - Repeat sequence
// 		   	  | INCH_5		// Input X = AX (e.g. INCH_0 = A0)
    		  | SHS_2		// Source select ; 0 - ADC10SC bit ; 1 - Timer_A.OUT1 ; 2 - Timer_A.OUT0
//    		  | ISSH		// Set if sample-input is inverted
      		  ;

      ADC10CTL1 |= highest_channel;
      ADC10DTC1 = number_of_conversions;                         // 3 conversions
      ADC10AE0 |= Xaxis+Yaxis+Zaxis;

//Timer_A configuration
          //Count up to measure_frequency
    TACCR0 = measure_time*125;

    	TACTL =
    			TASSEL_2	// 0 - TACLK ; 1 - ACLK ; 2 - SMCLK ; 3 - INCLK
    		  | MC_1 		// 0 - Stop ; 1 - Up ; 2 - Continuous ; 3 - Up/down
    		  | ID_3 		// 0 - CLK/1 ; 1 - CLK/2 ; 2 - CLK/4 ; 3 - CLK/8
//    	  	  | TACLR		// Timer_A Clear
//    		  | TAIE		// Timer_A interruption Enable - TAIFG flag
    		  ;


    	TACCTL0 =
//    			CCIE		// Enable CC interrupt - CCIFG flag
//    		  | CM_			// Capture Mode; 0 - No capture ; 1 - Rising edge ; 2 - falling edge ; 3 - Both edges
//    		  | CCIS_		// Capture input; 0 - CCIxA ; 1 - CCIxB ; 2 - GND ; 3 - Vcc
//    		  | SCS			// Synchronous capture
//    		  | SCCI		//
    		    OUTMOD_4		// PWM output mode; 0 - output only ; 1 - Set ; 2 - Toggle/reset ; 3 - Set/Reset ;
    						// 4 - Toggle ; 5 - reset ; 6 - Toggle/set ; 7 - Reset/set
    		  ;

//UART configuration
    	UCA0CTL1 =
    			UCSWRST;
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


    	send[0] = 'x';
    	send[9] = 'y';
    	send[18] = 'z';
    	send[1] = send[10] = send[19] = ':';
    	send[2] = send[7] = send[8] = send[11] = send[16] = send[17] = send[20] = ' ';
    	send[25] = '\r';
    	send[26] = 0x0B;

  while(1)
  {
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
    ADC10SA = (unsigned int)&adc[0];                        // Data buffer start                          // P1.0 = 1
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
    __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
    P1OUT ^= BIT6;                         // P1.0 = 0
//    for(i=0;i<10000;i++);
  }
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{	unsigned int pos=3;
	unsigned int j;

	for(i=0;i<number_of_conversions;i++){
		itoa(adc[i],buffer,10);
		for(j=0;j<4;j++){
			send[(pos+j)]=buffer[j];
			buffer[j]=' ';
		}
		pos=pos+9;
	}
	ADC10CTL0 &= ~ENC;
	IE2 = UCA0TXIE;
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void UART_interrupt(void){
    if(tx_count<=26)	UCA0TXBUF = send[ tx_count++ ];
    else{
    	IE2 &= ~UCA0TXIE;
    	tx_count=0;
    	__bic_SR_register_on_exit(CPUOFF);
    }

}

