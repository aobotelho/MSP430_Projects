#include "tempConfig.h"

void tempInit(){
	// Control Register 0. See page 553 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information.
	ADC10CTL0 =
				SREF_1			// Voltage Reference. 1- Vr+= Vref+ and Vr-= Vss;
				| ADC10SHT_3	// Sample and Hold time. 3- 64xADC10CLKs
//				| ADC10SR		// ADC10 sampling rate. 0- ~200ksps ; 1- ~50ksps
//				| REFOUT		// Reference output. 0- OFF ; 1- ON (devices with VeREF/VREF+ pins)
//				| REFBURST		// Reference burst. 0- continuosly ; 1- only during sample-and-conversion
//				| MSC			// Multiple sample and conversion. 0- all sample-and-conversion need a rising edge of SHI.
//				| REF2_5V		// Reference output. Need REFON to work. 0- 1.5V ; 1- 2.5V
				| REFON			// Reference generator. 0- OFF ; 1- ON
				| ADC10ON		// ADC10 on. 0- OFF	; 1- ON
//				| ADC10IE		// Interrupt Enable. 0- disabled ; 1- enabled
//				| ADC10IFG		// ADC10 Interrupt flag. 0- no interrupt ; 1- interrupt pending
//				| ENC 			// Enable conversion. 0- disabled ; 1- enabled
//				| ADC10SC		// Start Conversion. 0- no sample-and-conversion start ;
				;

	// Control Register 1. See page 555 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information.
	ADC10CTL1 =
				INCH_10			// Input channel select. 10- Temperature sensor
//				| SHS_0			// Sample-and-hold source select. 0- ADC10SC bit
//				| ADC10DF		// Data format. 0- straight binary ; 1- 2s complement
//				| ISSH			// Inverted signal sample-and-hold. 0- not inverted ; 1- inverted
				| ADC10DIV_3	// ADC10 clock divider. 3- /4
//				| ADC10SSEL_0	// Clock source. 0- ADC10OSC
//				| CONSEQ_0		// Conversion sequence mode. 0- single-channel-single-conversion
// READ ONLY	| ADC10BUSY		// ADC10 busy. If set there is an active sample in operation
				;

}

long getTemp(){
	long temp = 0;
	__delay_cycles(1000);
	ADC10CTL0 |= ENC + ADC10SC;
	while(ADC10CTL1 & ADC10BUSY);

	temp = ADC10MEM;

	ADC10CTL0 &= ~ENC;
	return  ((temp - 673) * 423) / 1024;
}
