#include "HomeAutomation.h"

#if defined HOME_AUTOMATION

unsigned int ListenPipe(unsigned int pipe){
	unsigned int counter = 0;

	switch(pipe){
		case PIPE_0:
			flush_rx();
			// Open Pipe0 without Enhanced ShockBurst
			msprf24_open_pipe(PIPE_0, 0);
			msprf24_activate_rx();
			do{
				if(msprf24_rx_pending()){
					LED4_PORT ^= LED4_IO;
					r_rx_payload(PACKET_SIZE,RXBuffer[0]);
					if(!msprf24_queue_state()&RF24_QUEUE_RXEMPTY){
						r_rx_payload(PACKET_SIZE,RXBuffer[1]);
						if(!msprf24_queue_state()&RF24_QUEUE_RXEMPTY){
							r_rx_payload(PACKET_SIZE,RXBuffer[2]);
							msprf24_close_pipe(PIPE_0);
							return 3;
						}
						else{
							msprf24_close_pipe(PIPE_0);
							return 2;
						}
					}
					else{
						msprf24_close_pipe(PIPE_0);
						return 1;
					}
				}
			}while(counter++ < DELAY_CYCLES_1MS);

			msprf24_close_pipe(PIPE_0);
			return 0;
		case PIPE_1:
			flush_rx();
			msprf24_activate_rx();
			// Open Pipe1 without Enhanced ShockBurst
			msprf24_open_pipe(PIPE_1, 0);
			do{
				if(msprf24_rx_pending()){
					r_rx_payload(PACKET_SIZE,RXBuffer[0]);
					if(!msprf24_queue_state()&RF24_QUEUE_RXEMPTY){
						r_rx_payload(PACKET_SIZE,RXBuffer[1]);
						if(!msprf24_queue_state()&RF24_QUEUE_RXEMPTY){
							r_rx_payload(PACKET_SIZE,RXBuffer[2]);
							msprf24_close_pipe(PIPE_1);
							return 3;
						}
						else{
							msprf24_close_pipe(PIPE_1);
							return 2;
						}
					}
					else{
						msprf24_close_pipe(PIPE_1);
						return 1;
					}
				}
			}while(counter++ < DELAY_CYCLES_1MS);

			msprf24_close_pipe(PIPE_1);
			return 0;
		case PIPE_2:
			flush_rx();
			msprf24_activate_rx();
			// Open Pipe1 without Enhanced ShockBurst
			msprf24_open_pipe(PIPE_2, 0);
			do{
				if(msprf24_rx_pending()){
					r_rx_payload(PACKET_SIZE,RXBuffer[0]);
					if(!msprf24_queue_state()&RF24_QUEUE_RXEMPTY){
						r_rx_payload(PACKET_SIZE,RXBuffer[1]);
						if(!msprf24_queue_state()&RF24_QUEUE_RXEMPTY){
							r_rx_payload(PACKET_SIZE,RXBuffer[2]);
							msprf24_close_pipe(PIPE_2);
							return 3;
						}
						else{
							msprf24_close_pipe(PIPE_2);
							return 2;
						}
					}
					else{
						msprf24_close_pipe(PIPE_2);
						return 1;
					}
				}
			}while(counter++ < DELAY_CYCLES_1MS);

			msprf24_close_pipe(PIPE_2);
			return 0;
		case PIPE_3:
			flush_rx();
			// Open Pipe1 without Enhanced ShockBurst
			msprf24_open_pipe(PIPE_3, 0);
			do{
				if(msprf24_rx_pending()){
					r_rx_payload(PACKET_SIZE,RXBuffer[0]);
					if(!msprf24_queue_state()&RF24_QUEUE_RXEMPTY){
						r_rx_payload(PACKET_SIZE,RXBuffer[1]);
						if(!msprf24_queue_state()&RF24_QUEUE_RXEMPTY){
							r_rx_payload(PACKET_SIZE,RXBuffer[2]);
							msprf24_close_pipe(PIPE_3);
							return 3;
						}
						else{
							msprf24_close_pipe(PIPE_3);
							return 2;
						}
					}
					else{
						msprf24_close_pipe(PIPE_3);
						return 1;
					}
				}
			}while(counter++ < DELAY_CYCLES_1MS);

			msprf24_close_pipe(PIPE_3);
			return 0;
		case PIPE_4:
			flush_rx();
			// Open Pipe1 without Enhanced ShockBurst
			msprf24_open_pipe(PIPE_4, 0);
			do{
				if(msprf24_rx_pending()){
					r_rx_payload(PACKET_SIZE,RXBuffer[0]);
					if(!msprf24_queue_state()&RF24_QUEUE_RXEMPTY){
						r_rx_payload(PACKET_SIZE,RXBuffer[1]);
						if(!msprf24_queue_state()&RF24_QUEUE_RXEMPTY){
							r_rx_payload(PACKET_SIZE,RXBuffer[2]);
							msprf24_close_pipe(PIPE_4);
							return 3;
						}
						else{
							msprf24_close_pipe(PIPE_4);
							return 2;
						}
					}
					else{
						msprf24_close_pipe(PIPE_4);
						return 1;
					}
				}
			}while(counter++ < DELAY_CYCLES_1MS);

			msprf24_close_pipe(PIPE_4);
			return 0;
		case PIPE_5:
			flush_rx();
			// Open Pipe1 without Enhanced ShockBurst
			msprf24_open_pipe(PIPE_5, 0);
			do{
				if(msprf24_rx_pending()){
					r_rx_payload(PACKET_SIZE,RXBuffer[0]);
					if(!msprf24_queue_state()&RF24_QUEUE_RXEMPTY){
						r_rx_payload(PACKET_SIZE,RXBuffer[1]);
						if(!msprf24_queue_state()&RF24_QUEUE_RXEMPTY){
							r_rx_payload(PACKET_SIZE,RXBuffer[2]);
							msprf24_close_pipe(PIPE_5);
							return 3;
						}
						else{
							msprf24_close_pipe(PIPE_5);
							return 2;
						}
					}
					else{
						msprf24_close_pipe(PIPE_5);
						return 1;
					}
				}
			}while(counter++ < DELAY_CYCLES_1MS);

			msprf24_close_pipe(PIPE_5);
			return 0;
		default:
			return 0;
	}

}

#endif	/*HOME_AUTOMATION*/
