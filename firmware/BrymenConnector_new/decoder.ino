#include "config.h"
#include "decoder.h"

byte data[20];
extern volatile bool rawData;

byte getByte(void)
{
	byte temp = 0;

	for (byte i = 0; i < 8; i ++)
	{
		IRLED_HIGH;
		_delay_us(CLOCK_DELAY);
		
		IRLED_LOW;
		if (!IRTRAN_LOW)
			temp |= (1 << i);
		_delay_us(CLOCK_DELAY);
	}

	return temp;
}

byte getData(void)
{
	IRLED_HIGH;
	_delay_ms(10);
	IRLED_LOW;

	byte timeout = 0xFF;
	while (!IRTRAN_LOW)
	{
		timeout--;
		if (!timeout)	return 0;
		_delay_us(2000);
	}

	for (byte i = 0; i < 20; i ++)
		data[i] = getByte();

	return 1;
}

void processData(void)
{
	if (rawData)
	{
		Serial.print("RAW DATA: ");
		
		for (byte i = 0; i < 20; i++)
		{
			Serial.print(data[i], HEX);
			Serial.print(' ');
		}

		Serial.print('\n');
		return;
	}

	//decode the output
	if (data[0] & 0x01)
		Serial.print("AUTO ");
	if ((data[0] >> 7) & 0x01)
		Serial.print("AVG ");
	if ((data[0] >> 6) & 0x01)
		Serial.print("MIN ");
	if ((data[0] >> 5) & 0x01)
		Serial.print("MAX ");
	if ((data[0] >> 2) & 0x01)
		Serial.print("CREST ");
	if ((data[0] >> 1) & 0x01)
		Serial.print("REC ");
	if ((data[0] >> 3) & 0x01)
		Serial.print("HOLD ");
	if (data[2] & 0x01)
		Serial.print("DELTA ");
	if (data[9] & 0x01)
		Serial.print("BEEP ");

	//DECODE MAIN DISPLAY
	Serial.print("MAIN: ");

	if ((data[1] >> 7) & 0x01)
		Serial.print('-');

	for (byte i = 0; i < 6; i++)
	{
		Serial.write(48 + decodeDigit(data[2 + i]));
		if ((data[3 + i] & 0x01) & (i < 4))
		Serial.print('.');
	}
	Serial.print(' ');
	
	//DECODE UNIT PREFIX FOR MAIN DISPLAY
	if ((data[13] >> 6) & 0x01)
		Serial.print('n');
	if ((data[14] >> 3) & 0x01)
		Serial.print('u');
	if ((data[14] >> 2) & 0x01)
		Serial.print('m');
	if ((data[14] >> 6) & 0x01)
		Serial.print('k');
	if ((data[14] >> 5) & 0x01)
		Serial.print('M');
	
	//DECODE UNIT FOR MAIN DISPLAY
	if (data[7] & 0x01)
		Serial.print('V');
	if ((data[13] >> 7) & 0x01)
		Serial.print('A');
	if ((data[13] >> 5) & 0x01)
		Serial.print('F');
	if ((data[13] >> 4) & 0x01)
		Serial.print('S');
	if ((data[14] >> 7) & 0x01)
		Serial.print("D%");
	if ((data[14] >> 4) & 0x01)
		Serial.print("Ohm");
	if ((data[14] >> 1) & 0x01)
		Serial.print("dB");
	if (data[14] & 0x01)
		Serial.print("Hz");
	Serial.print(' ');

	//DC OR AC
	if ((data[0] >> 4) & 0x01)
		Serial.print("DC ");
	if (data[1] & 0x01)
		Serial.print("AC ");

	//DECODE AUXILIARY DISPLAY
		Serial.print("AUX: ");

	if ((data[8] >> 4) & 0x01)
		Serial.print('-');

	for (byte i = 0; i < 4; i++)
	{
		Serial.write(48 + decodeDigit(data[9 + i]));
		if ((data[10 + i] & 0x01) & (i < 3))
		Serial.print('.');
	}
	
	Serial.print(' ');

	//DECODE UNIT PREFIX FOR AUXILIARY DISPLAY
	if ((data[8] >> 1) & 0x01)
		Serial.print('m');
	if (data[8] & 0x01)
		Serial.print('u');
	if ((data[13] >> 1) & 0x01)
		Serial.print('k');
	if (data[13] & 0x01)
		Serial.print('M');

	//DECODE UNIT FOR AUXILIARY DISPLAY
	if ((data[13] >> 2) & 0x01)
		Serial.print("Hz");
	if ((data[13] >> 3) & 0x01)
		Serial.print('V');
	if ((data[8] >> 2) & 0x01)
		Serial.print('A');
	if ((data[8] >> 3) & 0x01)
		Serial.print("%4-20mA");
		Serial.print(' ');
	if ((data[13] >> 2) & 0x01)
		Serial.print("AC ");

	//FINISH
	Serial.print('\n');

	//detect low battery
	if ((data[8] >> 7) & 0x01)
		Serial.println("LOW BAT\n");
}

char decodeDigit(byte source)
{
	char result = 0;

	switch (source >> 1)
	{
		case 0b1011111: result = 0; break;
		case 0b1010000: result = 1; break;
		case 0b1101101: result = 2; break;
		case 0b1111100: result = 3; break;
		case 0b1110010: result = 4; break;
		case 0b0111110: result = 5; break;
		case 0b0111111: result = 6; break;
		case 0b1010100: result = 7; break;
		case 0b1111111: result = 8; break;
		case 0b1111110: result = 9; break;
		case 0b1111001: result = ('d' - 48); break;
		case 0b0010000: result = ('i' - 48); break;
		case 0b0111001: result = ('o' - 48); break;
		case 0b0001011: result = ('L' - 48); break;
		case 0b0000000: result = (' ' - 48); break;
		default: result = ('?' - 48); break;
	}

	return result;
}
