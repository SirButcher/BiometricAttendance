// LcdScreen.h

#ifndef _LCDSCREEN_h
#define _LCDSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void SetupLCD();

void WriteToScreen(int, int);

void WriteToScreen(char[], char[]);

char* ReadFromFlash(int);

#endif


