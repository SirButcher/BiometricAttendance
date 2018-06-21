// NetModule.h

#ifndef _NETMODULE_h
#define _NETMODULE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void SetupNetModule();

int ConfirmBioID(int);

int GetNewID();

int GetCheck(int, char[17], char[17]);

int SendLogRequest(int, int);

int CheckConnection();


#endif



