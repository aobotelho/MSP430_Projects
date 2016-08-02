#include "Serial_LCD.h"


#if defined USE_SERIAL_LCD
void PutCharSerialLCD(char data){
	UARTVar.SendBuffer[UARTVar.BytesToBeSent] = data;

	if(UARTVar.BytesToBeSent == UART_BUFFER_SIZE-1){
		SEND_DATA;
	}
	else {
		UARTVar.BytesToBeSent++;
	}
}
void SerialLCDCommand(char command){
	PutCharSerialLCD(LCD_SPECIAL_COMMAND);
	PutCharSerialLCD(command);
}

void ClearSerialLCDLine(BOOL Line){
	unsigned int i;

	SerialLCDCommand(LCD_SET_CURSOR);
	PutCharSerialLCD(LCD_COLUMN);

	if(Line)
		PutCharSerialLCD(0x02);
	else
		// Send Cursor Home
		PutCharSerialLCD(0x01);

	for(i=0;i<LCD_COLUMN;i++)
		PutCharSerialLCD(LCD_BACKSPACE);

	SEND_DATA;
}

void ClearSerialLCD(void){
	// Clear Display
	SerialLCDCommand(LCD_CLEAR_DISPLAY);

	// Send Cursor Home
	SerialLCDCommand(LCD_CURSOR_HOME);

	SEND_DATA;
}

void SerialLCDInit(void){
	// Set Contrast
	SerialLCDCommand(LCD_SET_CONTRAST);
	PutCharSerialLCD(0xFF);

	// LCD Backlight ON/OFF
	SerialLCDCommand(LCD_BACKLIGHT_ON);
	PutCharSerialLCD(0x00);

	// General Purpose Output ON/OFF
	SerialLCDCommand(LCD_GPO_OFF);

	//Set Brightness
	SerialLCDCommand(LCD_SET_BRIGHT);
	PutCharSerialLCD(0xFF);

	// Blinking Cursor ON/OFF
	SerialLCDCommand(LCD_BLINKING_OFF);

	// Underline Cursor ON/OFF
	SerialLCDCommand(LCD_UNDERLINE_OFF);

	// Clear and sputcUARTend the cursor home
	ClearSerialLCD();

	// Backlight Color
	SerialLCDCommand(LCD_SET_RGB);
	PutCharSerialLCD(0xFF);	// Red
	PutCharSerialLCD(0x00);	// Green
	PutCharSerialLCD(0x00);	// Blue

	SEND_DATA;
}

void SerialLCDUpdateLine(BOOL Line,char *message){
	unsigned int i=0;

	ClearSerialLCDLine(Line);

	if(Line){
		// Jump to Second Line
		SerialLCDCommand(LCD_SET_CURSOR);
		PutCharSerialLCD(0x01);
		PutCharSerialLCD(0x02);
	}

	else{
		// Send Cursor Home
		SerialLCDCommand(LCD_CURSOR_HOME);
	}

	do{
		if(*message == '\0') break;
		PutCharSerialLCD(*message);
		i++;
	} while( *message++ && i < LCD_COLUMN);

	for(;i<LCD_COLUMN;i++)
		PutCharSerialLCD(' ');

	SEND_DATA;
}

#endif
