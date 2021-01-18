/*
 * BrymenConnector_new.cpp
 * Info: adapter, used to send data from a Brymen 867/9 to a PC using UART
 * Author : martin@embedblog.eu
 * More info: http://embedblog.eu/?p=475
 * IC: ATtiny45
 * Fuses: L0x02 H0xDF E0xFF
 * License:  CC BY-SA 4.0 International
 */ 


#include "config.h"

volatile uint8_t mode = MODE_DEFAULT;
volatile bool rawData = false;

#include "decoder.h"



void setup()
{
	//setup IO - output for the IR  & activity LEDs

	pinMode(pinIR,OUTPUT);
	pinMode(pinLED,OUTPUT);

	//input for the phototransistor

	pinMode(pinTransistor,INPUT);

	//give the serial terminal a while to react
	delay(500);		
	Serial.begin(UART_BAUD);


	Serial.println("Brymen 867/869 interface cable, JarnoKai fork");
	Serial.println("For more info, see embedblog.eu/?p=475 and github.com/JarnoKai/brymen-867-interface-cable");
	Serial.println("Available commands:");
	Serial.println("F - 5 samples per second");
	Serial.println("O - 1 sample per second");
	Serial.println("S - stop autosend");
	Serial.println("D - send a single reading");
	Serial.println("R - toggle raw data output");
}

void loop()
{
	if (Serial.available())
	{
		byte data = Serial.read();
		switch (data)
		{
			case 'R': rawData = !rawData; break;
			case 'F': mode = MODE_5X_PER_SEC; break;
			case 'O': mode = MODE_1X_PER_SEC; break;
			case 'S': mode = MODE_STOP; break;
			case 'D': mode = MODE_SEND; break;
			default: break;
		}
		while(Serial.available())
			Serial.read();
	}
	if (getData())
	{
		LED_HIGH;
		processData();
		LED_LOW;

		if (mode == MODE_1X_PER_SEC)
			_delay_ms(920);

		if ((mode == MODE_STOP) | (mode == MODE_SEND))
		{
			mode = MODE_STOP;
			while (mode == MODE_STOP);
		}
	}
}

