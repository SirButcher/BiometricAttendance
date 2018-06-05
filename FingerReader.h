// FingerReader.h

#ifndef _FINGERREADER_h
#define _FINGERREADER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void SetupReader();

void EnrollFinger(uint8_t);

int getFingerprintID();

#endif

