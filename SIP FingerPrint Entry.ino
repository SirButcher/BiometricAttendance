
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

#include "FingerReader.h"
#include "NetModule.h"
#include "LcdScreen.h"

char dataLines[32] =	" Hello, Gabriel Last event: Out";

char screenReady[32] =	" System booting   Please wait  ";
char netIsFail[32] =	" Net is tested    FATAL ERROR! ";

char systemStdb[32] =	"System is ready Use the reader ";
								//		 |
void setup()
{
	// First of all, open the serial interface
	// for debugging
	Serial.begin(9600);

	Serial.println("App is starting!");

	

	// First, setup the LCD screen.
	// This will initialize it.
	SetupLCD();
	WriteToScreen(&screenReady[0]);

	SetupReader();

	SetupNetModule();

	delay(15000);

	if (!CheckConnection())
	{
		// Connection failed.
		WriteToScreen(&netIsFail[0]);
		while (1) { delay(1); }
	}

	delay(2000);

	WriteToScreen(&systemStdb[0]);

	delay(2000);



	getFingerprintID();

	//EnrollFinger(2);
}

void loop()
{

  /* add main program code here */

}
