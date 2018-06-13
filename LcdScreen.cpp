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

void SetupLCD()
{
	Serial.println("Initializing LCD");

	lcd.init();
	lcd.backlight();

	Serial.println("LCD is OK!");
}

void WriteToScreen(char data[], int line)
{
	lcd.setCursor(0, line);
	lcd.print(data);
}

void WriteToScreen(char firstLine[], char secondLine[])
{
	Serial.println("Writing to the screen: ");
	Serial.print('\'');
	Serial.print(firstLine);
	Serial.print('\'');

	Serial.println('\'');
	Serial.print(secondLine);
	Serial.print('\'');
	Serial.println();



	lcd.setCursor(0, 0);
	lcd.print(firstLine);


	lcd.setCursor(0, 1);
	lcd.print(secondLine);
}
