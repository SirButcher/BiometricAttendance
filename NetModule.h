// NetModule.h

#ifndef _NETMODULE_h
#define _NETMODULE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void SetupNetModule();

int GetNewID();

int GetCheck(int, char[32]);

void Login(int);
void Logout(int);

int CheckConnection();


#endif



