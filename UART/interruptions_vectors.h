#ifndef INTERRUPTIONS_VECTORS_H_
#define INTERRUPTIONS_VECTORS_H_
#include "Hardware.h"

#if defined USE_UART
	extern UART_VAR UARTVar;
#endif

#if defined USE_SPI_USCIA || defined USE_SPI_USCIB

	#if defined USE_SPI_USCIA
		extern SPI_VAR SPIVarA;
	#endif

	#if defined USE_SPI_USCIB
		extern SPI_VAR SPIVarB;
	#endif

#endif


#if defined USE_NRF24L01
	#include "msprf24.h"
	volatile unsigned char rf_irq;
#endif

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void){
	#if defined UART_USE_TX_INTERRUPT
		if(UARTVar.SendByteCounter<UARTVar.BytesToBeSent){
			UCA0TXBUF = UARTVar.SendBuffer[UARTVar.SendByteCounter];
			UARTVar.SendByteCounter++;
		}
		else{
			IE2 &= ~UCA0TXIE;
			UARTVar.SendByteCounter=0;
			UARTVar.BytesToBeSent=0;
			__bic_SR_register_on_exit(CPUOFF);
		}
	#endif

	#if defined SPI_USE_TX_INTERRUPT && defined USE_SPI_USCIA
	#endif

	#if defined SPI_USE_TX_INTERRUPT && defined USE_SPI_USCIB
	#endif

	#if I2C_USE_TX_INTERRUPT
	#endif
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void) {
	#if defined UART_USE_RX_INTERRUPT
		if(!UARTVar.NewData) UARTVar.NewData = 1;
		UARTVar.ReceiveBuffer[UARTVar.BytesReceived++] = UCA0RXBUF;
		if(UARTVar.BytesReceived == UART_BUFFER_SIZE){
			IE2 &= ~UCA0RXIE;
			__bic_SR_register_on_exit(LPM0_bits);
		}
	#endif

	#if defined SPI_USE_RX_INTERRUPT && defined USE_SPI_USCIA && defined USE_NRF24L01
		IE2 &= ~UCA0RXIE;
		__bic_SR_register_on_exit(LPM0_bits);
	#endif

	#if defined SPI_USE_RX_INTERRUPT && defined USE_SPI_USCIB && defined USE_NRF24L01
		IE2 &= ~UCB0RXIE;
		__bic_SR_register_on_exit(LPM0_bits);
	#endif

	#if defined I2C_USE_RX_INTERRUPT
	#endif
}



#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void){while(1);}
#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR (void){while(1);}
#pragma vector = COMPARATORA_VECTOR
__interrupt void COMPARATORA_ISR(void){while(1);}
#pragma vector = NMI_VECTOR
__interrupt void NMI_ISR(void){while(1);}


#pragma vector = PORT2_VECTOR
__interrupt void P2_IRQ (void) {
	#if defined USE_NRF24L01
		#if nrfIRQport == 2
			if(P2IFG & nrfIRQpin){
			__bic_SR_register_on_exit(LPM4_bits);    // Wake up
			rf_irq |= RF24_IRQ_FLAGGED;
			P2IFG &= ~nrfIRQpin;   // Clear interrupt flag
			}
		#endif
	#endif
}


#pragma vector = PORT1_VECTOR
__interrupt void P1_IRQ (void){while(1);}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void){while(1);}
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void){while(1);}
#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void){while(1);}
#pragma vector = TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void){while(1);}



#endif /* INTERRUPTIONS_VECTORS_H_ */
