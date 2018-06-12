
// SIP Entry system, using fingerprint and arduino.

// Needed modules:

// 1. Connecting to the SIP server to gather data - NET module
// 2. Reading button presses - STAGES module
// 3. Writing data to the screen - SCREEN module
// 4. Reading fingerprint and enrolling fingerprint

// URLs available for the server:

// https://phone.sipcarparks.co.uk/Entry/BioEntry.ashx?PSW=z3He8mMKwSw7yGch&TYPE=NEW - Return a number
// https://phone.sipcarparks.co.uk/Entry/BioEntry.ashx?PSW=z3He8mMKwSw7yGch&TYPE=NEWDONE - Return nothing
// https://phone.sipcarparks.co.uk/Entry/BioEntry.ashx?PSW=z3He8mMKwSw7yGch&TYPE=CHECK&BIOID=0 - return 2x16 character
// https://phone.sipcarparks.co.uk/Entry/BioEntry.ashx?PSW=z3He8mMKwSw7yGch&TYPE=LOGIN&BIOID=0 - Login
// https://phone.sipcarparks.co.uk/Entry/BioEntry.ashx?PSW=z3He8mMKwSw7yGch&TYPE=LOGOUT&BIOID=0 - Logout

// LCD screen cabling:
// Put the SCL to the A5 ( Orange )
// Put the SDA to the A4 ( White )
// It uses 5V!!


#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <Ethernet.h>

#include "App_Constants.h"
#include "FingerReader.h"
#include "NetModule.h"
#include "LcdScreen.h"

char dataLines[32] =	"                               ";

char _systemBooting[17] = " System booting ";
char _netFailed[17] = "  Net is failed ";
char _systemReady[17] = "System is ready ";
char _pleaseWait[17] = "   Please wait  ";
char _useTheReader[17] = " Use the reader ";

char _tryAgain[17] = "    Try again   ";

char _imageTook[17] = "   Image taken  ";
char _removeFinger[17] = "  remove finger ";


char _enroll0[17] = "Put same finger ";
char _enroll1[17] = " on the reader  ";

char _userStored0[17] = "  Success! User ";
char _userStored1[17] = " stored. ID:xxx ";


// Errors
char _fatalError[17] = "   FATAL ERROR  ";
char _systemError[17] = " System error!  ";
char _imageError[17] = "  Image error!  ";
char _imageNotMatch[17] = "Image not match ";
char _userNotFound[17] = " User not found ";


int userID = -1;


void setup()
{
	// First of all, open the serial interface
	// for debugging
	Serial.begin(9600);

	Serial.println("App is starting!");

	

	// First, setup the LCD screen.
	// This will initialize it.
	SetupLCD();
	WriteToScreen(_systemBooting, 0);
	WriteToScreen(_pleaseWait, 1);

	SetupReader();

	SetupNetModule();

	delay(15000);

	if (!CheckConnection())
	{
		// Connection failed.
		WriteToScreen(_netFailed, 0);
		WriteToScreen(_fatalError, 1);

		while (1) { delay(1); }
	}

	WriteToScreen(_systemReady, 0);
	WriteToScreen(_useTheReader, 1);
}

void loop()
{
	// Okay, in this main loop we will have two task:
	// 1. Check if we have a finger on the reader
	// 2. Check if the user pressed both the buttons (entering to the Enroll)

	// This method will return with either -1 (no finger on the screen)
	// or with the user's ID.

	/*

	userID = getFingerprintID();

	if (userID > -1)
	{
		// We got a user data!
		// Now get the CHECK data from the server

		Serial.print("Found a fingerprint with ID ");
		Serial.println(userID);

		if (GetCheck(userID, dataLines))
		{
			// Success!
			WriteToScreen(_&dataLines[0]); 

			// Waiting for the RED or GREEN button
		}
		else
		{
			// Connection error happened!
			WriteToScreen(_&netIsFail[0]);

			// This count as a fatal error
			while (1) { delay(1); }
		}
	}
	else if(userID < -1) // Error happened - write the regular screen
	{
		WriteToScreen(_&systemStdb[0]);
	}

	*/

	// Rest a little.
	delay(50);
}
