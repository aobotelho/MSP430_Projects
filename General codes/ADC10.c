#include "ADC10.h"

void ADCInit(void){
	//******************************************************
	//	ADC10CTL0,: see page 553 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
	//		- SREF_ : Voltage Reference
 	//			+ SREF_0: VR+ = AVCC and VR- = AVSS
	//			+ SREF_1: VR+ = VREF+ and VR- = AVSS
	//			+ SREF_2: VR+ = VEREF+ and VR- = AVSS
	//			+ SREF_3: VR+ = VEREF+ and VR- = AVSS
	//			+ SREF_4: VR+ = AVCC and VR- = VREF-/VEREF-
	//			+ SREF_5: VR+ = VREF+ and VR- = VREF-/VEREF-
	//			+ SREF_6: VR+ = VEREF+ and VR- = VREF-/VEREF-
 	//			+ SREF_7: VR+ = VEREF+ and VR- = VREF-/VEREF-
	//		- ADC10SHT_ : ADC10 sample-and-hold time
 	//			+ ADC10SHT_0: 4 x ADC10CLKs
 	//			+ ADC10SHT_1: 8 x ADC10CLKs
 	//			+ ADC10SHT_2: 16 x ADC10CLKs
 	//			+ ADC10SHT_3: 64 x ADC10CLKs
	//		- ADC10SR: ADC10 Sampling Rate
	//			+ If set: Reference buffer supports up to ~50kbps
	//			+ If not set: Reference buffer supports up to ~200kbps
	//		- REFOUT: Enable reference output
	//		- REFBURST: Reference Burst
	//			+ If set: Reference buffer on only during sample-and-conversion
	//			+ If not set: Reference buffer on continuously
	//		- MSC: Multiple sample and conversion. (Valid only for sequence or repeated modes)
	//			+ If set: The sampling requires a rising edge of the SHI signal to trigger each sample-and-conversion.
	//			+ If not set: The first rising edge of the SHI signal triggers the sampling timer, but further sample-andconversions are
	//						  performed automatically as soon as the prior conversion is completed
	//		- REF2_5V: Reference-generator voltage. (REFON must also be set)
	//			+ If set: 2.5 V
	//			+ If not set: 1.5 V
	//		- REFON: Reference generator on
	//			+ If set: Reference on
	//			+ If not set: Reference off
	//		- ADC10ON: ADC10 on
	//			+ If set: ADC10 on
	//			+ If not set: ADC10 off
	//		- ADC10IE: ADC10 interrupt enable
	//			+ If set: Interrupt enabled
	//			+ If not set: Interrupt disabled
	//		- ADC10IFG: ADC10 interrupt flag
	//		- ENC: Enable conversion
	//		- ADC10SC: Start sample-and-conversion
	//******************************************************
	ADC10CTL0 = SREF_0 | ADC10SHT_0 | ADC10ON;

	//******************************************************
	//	ADC10CTL1:see page 555 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
	//		- INCH_ : Input channel select. (See device specific data sheet)
	//			+ INCH_0: A0
	//			+ INCH_1: A1
	//			+ INCH_2: A2
	//			+ INCH_3: A3
	//			+ INCH_4: A4
	//			+ INCH_5: A5
	//			+ INCH_6: A6
	//			+ INCH_7: A7
	//		- SHS_ : Sample-and-hold source select.
	//			+ SHS_0: ADC10SC bit
	//			+ SHS_1: TIMER_A.OUT1
	//			+ SHS_2: TIMER_A.OUT2
	//		- ADC10DF : ADC10 data format
	//			+ If set: 2's complement
	//			+ If not set: Straight binary
	//		- ISSH: Invert signal sample-and-hold
	//			+ If set: The sample-input signal is inverted.
	//			+ If not set: The sample-input signal is not inverted.
	//		- ADC10DIV_: ADC10 clock divider
	//			+ ADC10DIV_0: /1
	//			+ ADC10DIV_1: /2
	//			+ ADC10DIV_2: /3
	//			+ ADC10DIV_3: /4
	//			+ ADC10DIV_4: /5
	//			+ ADC10DIV_5: /6
	//			+ ADC10DIV_6: /7
	//			+ ADC10DIV_7: /8
	//		- ADC10SSEL_: Clock Source
	//			+ ADC10SSEL_0: ADC10OSC
	//			+ ADC10SSEL_1: ACLK
	//			+ ADC10SSEL_2: MCLK
	//			+ ADC10SSEL_3: SMCLK
	//		- CONSEQ_: Conversion mode select
	//			+ CONSEQ_0: Single channel single conversion
	//			+ CONSEQ_1: Sequence of channels
	//			+ CONSEQ_2: Repeat single channel
	//			+ CONSEQ_3: Repeat sequence of channels
	//		- ADC10BUSY: ADC10 busy. This bit indicates an active sample or conversion operation
	//******************************************************
	ADC10CTL1 = INCH_2 | SHS_0 | ADC10DIV_0 | ADC10SSEL_3 | CONSEQ_1;

	//******************************************************
	//	ADC10AE0:see page 556 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
	//		- ADC10AE0 : ADC10 analog enable. These bits enable the corresponding pin for analog input. BIT0 corresponds to A0,	BIT1 corresponds to A1, etc.
	//******************************************************
	ADC10AE0 = BIT0 | BIT1 | BIT2 ;

	//******************************************************
	//	ADC10DTC1:Data Transfer Control Register 1
	//		- DTC transfers. These bits define the number of transfers in each block.
	//			+ 01h-0FFh - Number of transfers per block
	//******************************************************
	ADC10DTC1 = 0x03;

	//******************************************************
	// ADC10MEM : The 10-bit conversion results
	//		- If in binary: The 10-bit conversion results are right justified, straight-binary format. Bit 9 is the MSB. Bits 15-10 are always 0.
	//		- If in 2's complement: The 10-bit conversion results are left-justified, 2s complement format. Bit 15 is the MSB. Bits 5-0 are always 0.
	//******************************************************
	ADC10MEM; // Read only

	//******************************************************
	// ADC10SA: Start Address Register for Data Transfer
	//		- ADC10 start address. These bits are the start address for the DTC. A write to register ADC10SA is required to initiate DTC transfers.
	//******************************************************
	ADC10SA = 0x200;

}
