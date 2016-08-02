// MSP430 MPU6050/6000 Example Startup Code
// 07/05/2012 by Mushfiq Sarker <mushfiqsarker@inventige.com>

// Changelog:
// ... - ongoing debug release

// NOTE: THIS IS ONLY A PARIAL RELEASE. THIS DEVICE CLASS IS CURRENTLY UNDERGOING ACTIVE
// DEVELOPMENT AND IS STILL MISSING SOME IMPORTANT FEATURES. PLEASE KEEP THIS IN MIND IF
// YOU DECIDE TO USE THIS PARTICULAR CODE FOR ANYTHING.

/* ============================================
MSP430 MPU6050/6000 Example Startup code is placed under the MIT license
Copyright (c) 2012 Mushfiq Sarker

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "msp430g2553.h"
//#include "msp430x22x4.h"					// Change this include to be specific to your particular MCU from TI.
#include <stdint.h>

// Include I2C MSP430 and MPU6050 libraries.
//#include "I2C_MSP430.h"						// Use the correct I2C library particular to your MCU.
#include "MSP430_MPU6050.h"
#include "MPU6050.h"

static int16_t ax, ay, az;					//<! Holds the raw accelerometer data.
static int16_t gx, gy, gz;					//<! Holds the raw gyroscope data.

void serial_setup(unsigned out_mask, unsigned in_mask, unsigned duration);

void printf(char *, ...);

int main(void)
{

	  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
	  BCSCTL1 = CALBC1_1MHZ; 				// 16MHZ operation
	  DCOCTL = CALDCO_1MHZ;

	  //Placed 10k or 4.7k ohm pullup resistors on SDA and SCL lines
      //P3DIR |= 0x0F;
	  //P3SEL |= 0x06;                        // Assign I2C pins to USCI_B0

	  P1DIR |= BIT0;                            // P1.0 output
	  P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
	  P1SEL2|= BIT6 + BIT7;

	 initializeIMU();						// Initialize IMU

	  msDelay(30);							// Temporary wait. Can be shortened
	  // Setup the serial port
	  // Serial out: P1.1 (BIT1)
	  // Serial in:  P1.2 (BIT2)
	  // Bit rate:   9600 (CPU freq / bit rate)
	  serial_setup(BIT1, BIT2, 1000000 / 9600);

	  printf("\r\n %s \r\n", "Motion");
	  for (;;)
	  	{
			getMotion6( &ax, &ay, &az, &gx, &gy, &gz);
			// ax, ay, az, gx, gy, gz have RAW values from accelerometer and gyroscope
			printf("%i %i %i %i %i %i\r\n", ax, ay, az, gx, gy, gz);

	  	}
}
