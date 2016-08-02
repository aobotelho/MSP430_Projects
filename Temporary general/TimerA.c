#include "TimerA.h"

void TimerAInit(void){
	//******************************************************
	//	TACTL: see page 370 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
	//		- TASSEL_ : Clock Source
	//			+ TASSEL_0: TACLK
	//			+ TASSEL_1: ACLK
	//			+ TASSEL_2: SMCLK
	//			+ TASSEL_3: INCLK
	//		- ID_ : Input Divider
	//			+ ID_0: /1
	//			+ ID_1: /2
	//			+ ID_2: /4
	//			+ ID_3: /8
	//		- MC_ : Mode Control
	//			+ MC_0: Timer Halted
	//			+ MC_1: Up mode (Up to TACCR0)
	//			+ MC_2: Continuous Mode (Up to 0xFFFF)
	//			+ MC_3: Up/Down mode (Up to TACCR0)
	//		- TACLR: Clear TimerA
	//		- TAIE: Interrupt Enable
	//		- TAIFG: Interrupt Flag
	//******************************************************

	TACTL = TASSEL_2 | ID_3 | MC_1 ;

	//******************************************************
	//	TACCTL0:see page 372 of MSP430x2xx Family User's Guide (http://www.ti.com/lit/ug/slau144j/slau144j.pdf) for more information
	//		- CM_ : Capture Mode
	//			+ CM_0: No Capture
	//			+ CM_1: Rising Edge
	//			+ CM_2: Falling Edge
	//			+ CM_3: Both Edges
	//		- CCIS_ : Capture/Compare Input Select
	//			+ CCIS_0: CCIxA
	//			+ CCIS_1: CCIxB
	//			+ CCIS_2: GND
	//			+ CCIS_3: Vcc
	//		- SCS : If set Synchronous Capture is set
	//		- SCCI: Synchronized capture/compare input
	//		- CAP: Capture Mode Enable (if not set Compare Mode is Enabled)
	//		- OUTMOD_: Output Mode
	//			+ OUTMOD_0: OUT bit Value
	//			+ OUTMOD_1: Set
	//			+ OUTMOD_2: Toggle/Reset (Not useful for TACCR0)
	//			+ OUTMOD_3: Set/Reset (Not useful for TACCR0)
	//			+ OUTMOD_4: Toggle
	//			+ OUTMOD_5: Reset
	//			+ OUTMOD_6: Toggle/Set (Not useful for TACCR0)
	//			+ OUTMOD_7: Reset/Set (Not useful for TACCR0)
	//		- CCIE: Enable Capture/Compare interrupt
	//		- CCI Capture/Compare input
	//		- OUT: Output High if set, low if not (For output mode 0 only).
	//		- COV: Bit that indicates if an overflow occurred
	//		- CCIFG: Capture/Compare Interrupt Flag
	//******************************************************
	//TACCTL0 = CCIE ;

	TACCR0 = 0xF424;
}
