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
	volatile unsigned char rf_irq;
#endif

#if defined USE_TIMERA
#include "TimerA.h"
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


#pragma vector = PORT2_VECTOR
__interrupt void P2_IRQ (void) {
	#if defined USE_NRF24L01
		unsigned int Pipe;
		#if nrfIRQport == 2
			if(P2IFG & nrfIRQpin){
			rf_irq |= RF24_IRQ_FLAGGED;
			P2IFG &= ~nrfIRQpin;   // Clear interrupt flag
			}
		#endif
			Pipe = r_rx_payload(PACKET_SIZE,RXBuffer[0]);
			switch(Pipe){
				case PIPE_0:
					LED1_PORT ^= LED1_IO;
					break;
				case PIPE_1:
					LED2_PORT ^= LED2_IO;
					break;
				case PIPE_2:
					LED3_PORT ^= LED3_IO;
					break;
				case PIPE_3:
					LED4_PORT ^= LED4_IO;
					break;
				case PIPE_4:
					LED5_PORT ^= LED5_IO;
					break;
				case PIPE_5:
					LED6_PORT ^= LED6_IO;
					break;
				default:
					break;
			}
	#endif
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void){
	P1OUT ^= BIT6;
	TACCTL0 &= ~CCIE ;
}

#pragma vector = PORT1_VECTOR
__interrupt void P1_IRQ (void){
	P1IFG &= ~0x08;
	P1OUT  |= BIT0;
	TACCTL0 |= CCIE ;
}
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void){while(1);}
#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR (void){while(1);}
#pragma vector = COMPARATORA_VECTOR
__interrupt void COMPARATORA_ISR(void){while(1);}
#pragma vector = NMI_VECTOR
__interrupt void NMI_ISR(void){while(1);}
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void){while(1);}
#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void){while(1);}
#pragma vector = TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void){while(1);}



#endif /* INTERRUPTIONS_VECTORS_H_ */
