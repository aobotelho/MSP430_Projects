#ifndef SERIAL_LCD_H_
#define SERIAL_LCD_H_

#include "Hardware.h"
#include "UARTConfig.h"

#if defined USE_SERIAL_LCD
	#if defined not USE_UART
		#pragma ENABLE_UART_TO_USE_SERIAL_LCD
	#else

		#define LCD_COLUMN	16u
		#define LCD_ROW		2u

		#define LCD_NEW_LINE		0x0A
		#define LCD_BACKLIGHT_ON	0x42	// + minutes ; if minutes = 0 backlight is on indefinitely
		#define LCD_BACKLIGHT_OFF	0x46
		#define LCD_SET_CURSOR		0x47	// + Column + Row ; Home = (1,1)
		#define LCD_CURSOR_HOME		0x48
		#define LCD_UNDERLINE_ON	0x4A
		#define LCD_UNDERLINE_OFF	0x4B
		#define LCD_CURSOR_LEFT		0x4C
		#define LCD_CURSOR_RIGHT	0x4D
		#define LCD_SET_CONTRAST	0x50	// + Constrast; 1 byte long (max: 0xFF)
		#define LCD_AUTOSCROLL_ON	0x51
		#define LCD_AUTOSCROLL_OFF	0x52
		#define LCD_BLINKING_ON		0x53
		#define LCD_BLINKING_OFF	0x54
		#define LCD_GPO_ON			0x56
		#define LCD_GPO_OFF			0x57
		#define LCD_CLEAR_DISPLAY	0x58
		#define LCD_SET_BRIGHT		0x99	// + brightness; 1 byte long (max: 0xFF)
		#define LCD_SET_RGB			0xD0	// + Red + Green + Blue; 1 byte long (max: 0xFF)
		#define	LCD_SET_SIZE		0xD1	// + Column + Row ;

		#define LCD_SPECIAL_COMMAND	0xFE

		#define LCD_BACKSPACE		0x08

		#define Line1	FALSE
		#define Line2	TRUE

		void PutCharSerialLCD(char data);
		void SerialLCDCommand(char command);
		void ClearSerialLCDLine(BOOL Line);
		void ClearSerialLCD(void);
		void SerialLCDInit(void);
		void SerialLCDUpdateLine(BOOL Line,char *message);


	#endif
#endif


#endif /* SERIAL_LCD_H_ */
