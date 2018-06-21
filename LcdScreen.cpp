// 
// 
// 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LcdScreen.h"

LiquidCrystal_I2C lcd(0x20, 16, 2);

char buffer[17] = "                ";

const char start_0[] PROGMEM = " System booting ";
const char start_1[] PROGMEM = "  Net is failed ";
const char start_2[] PROGMEM = " Hi, I'm Freddy ";
const char start_3[] PROGMEM = "   Please wait  ";
const char start_4[] PROGMEM = "   Finger Me ;) ";

const char gui_0[] PROGMEM = " Entering Setup ";
const char gui_1[] PROGMEM = "    Logged in   ";
const char gui_2[] PROGMEM = "   Logged out   ";
const char gui_3[] PROGMEM = "   Thank you!   ";
const char gui_4[] PROGMEM = " Getting userID ";
const char gui_5[] PROGMEM = "    Success!    ";
const char gui_6[] PROGMEM = "  Put finger on ";
const char gui_7[] PROGMEM = "   the reader   ";

const char notif_0[] PROGMEM = "    Try again   ";
const char notif_1[] PROGMEM = "   Image taken  ";
const char notif_2[] PROGMEM = "  remove finger ";
const char notif_3[] PROGMEM = "Put same finger ";
const char notif_4[] PROGMEM = " on the reader  ";

const char error_0[] PROGMEM = " System error!  ";
const char error_1[] PROGMEM = "  Image error!  ";
const char error_2[] PROGMEM = "Image not match ";
const char error_3[] PROGMEM = " User not found ";
const char error_4[] PROGMEM = "   FATAL ERROR  ";
const char error_5[] PROGMEM = " Man. Conf. Req ";
const char error_6[] PROGMEM = "  Reader error! ";

const char Empty[] PROGMEM =  "                ";

// Then set up a table to refer to your strings.

const char* const string_table[] PROGMEM =
{
	start_0, start_1, start_2, start_3, start_4,
	gui_0, gui_1, gui_2, gui_3,

	notif_0, notif_1, notif_2, notif_3, notif_4,

	error_0, error_1, error_2, error_3, error_4,

	Empty,

	gui_4, gui_5, error_5, gui_6, gui_7, error_6
};

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

void WriteToScreen(int firstMemSlot, int secondMemSlot)
{
	lcd.clear();

	Serial.println("Writing to the screen: ");
	Serial.print('\'');
	Serial.print(ReadFromFlash(firstMemSlot));
	Serial.println('\'');

	Serial.print('\'');
	Serial.print(ReadFromFlash(secondMemSlot));
	Serial.println('\'');

	Serial.println();

	lcd.setCursor(0, 0);
	lcd.print(ReadFromFlash(firstMemSlot));


	lcd.setCursor(0, 1);
	lcd.print(ReadFromFlash(secondMemSlot));
}

void WriteToScreen(char fistLine[], char secondLine[])
{
	lcd.clear();

	Serial.println("Writing to the screen: ");
	Serial.print('\'');
	Serial.print(fistLine);
	Serial.println('\'');

	Serial.print('\'');
	Serial.print(secondLine);
	Serial.println('\'');

	Serial.println();

	lcd.setCursor(0, 0);
	lcd.print(fistLine);


	lcd.setCursor(0, 1);
	lcd.print(secondLine);
}

char* ReadFromFlash(int index)
{
	strcpy_P(buffer, (char*)pgm_read_word(&(string_table[index])));

	return buffer;
}
