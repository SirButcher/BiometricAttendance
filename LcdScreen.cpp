// 
// 
// 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LcdScreen.h"

LiquidCrystal_I2C lcd(0x20, 16, 2);

// Put the SCL to the A5
// Put the SDA to the A4
// It uses 5V!!

char firstLine[16];
char secondLine[16];

void SetupLCD()
{
	Serial.println("Initializing LCD");

	lcd.init();
	lcd.backlight();

	Serial.println("LCD is OK!");
}

void WriteToScreen(char *data) 
{
	Serial.print("Writing to the screen: ");

	SplitData(data);

	Serial.println();
	Serial.print('\'');
	Serial.print(firstLine);
	Serial.print('\'');
	Serial.println();
	Serial.print('\'');
	Serial.print(secondLine);
	Serial.println('\'');
	Serial.println();

	lcd.setCursor(0, 0);
	lcd.print(firstLine);

	lcd.setCursor(0, 1);
	lcd.print(secondLine);
}

void SplitData(char *data)
{
	for (int i = 0; i < 16; i++)
	{
		firstLine[i] = *(data + i);
	}

	for (int i = 0; i < 16; i++)
	{
		secondLine[i] = *(data + 16 + i);
	}
	
}
