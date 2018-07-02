
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
// It uses 5V

// Fingerprint reader cabling:
// Green cable - Pin 4
// White cable - Pin 3
// It uses 5V

// Button cabling:
// Green button - Pin 8
// Red button	- Pin 7

// https://www.arduino.cc/reference/en/language/variables/utilities/progmem/


#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <Ethernet.h>

#include "App_Constants.h"
#include "FingerReader.h"
#include "NetModule.h"
#include "LcdScreen.h"

int buttonState = 0;
int greenButtonPort = 8;
int redButtonPort = 7;


char data_Line0[17] = "                ";
char data_Line1[17] = "                ";

// ---------- Initializing GLOBAL variables ----------

char _userStored0[17] = "  Success! User ";
char _userStored1[17] = " stored. ID:xxx ";

// Data stored in the PROGMEM Slot - in the LcdScreen.cpp

int _systemBooting = 0;
int _netFailed = 1;
int _welcome = 2;
int _pleaseWait = 3;
int _useTheReader = 4;
int _enteringSetup = 5;

int _LoggedIn = 6;
int _LoggedOut = 7;
int _thankYou = 8;


int _tryAgain = 9;

int _imageTook = 10;
int _removeFinger = 11;


int _enroll0 = 12;
int _enroll1 = 13;

// Errors
int _systemError = 14;
int _imageError = 15;
int _imageNotMatch = 16;
int _userNotFound = 17;
int _FatalError = 18;

int _EmptyLine = 19;

int _gettingID = 20;
int _Success = 21;
int _ManualConfReq = 22;

int _setupFirstStep = 23;
int _setupSecondStep = 24;

int _readerError = 25;

// ---------------------------------------------------

enum AppStage
{
	_StandBy,

	// Default stage, waiting for the user
	_WaitingForReader,

	// After a successful print reading
	_WaitingForButtonAfterUserRead,

	_RegisteringUser
};

enum ButtonPress
{
	_None,

	_RedButton,
	_GreenButton,


	_BothButton,

};

int userID = -1;
AppStage stage = _StandBy;


void setup()
{
	// First of all, open the Serial interface
	// for debugging
	Serial.begin(9600);

	Serial.println("App is starting!");

	pinMode(redButtonPort, INPUT);
	pinMode(greenButtonPort, INPUT);


	// Setup the LCD screen.
	// After the setup, we can write to the screen!
	SetupLCD();
	WriteToScreen(_systemBooting, _pleaseWait);

	SetupReader();

	SetupNetModule();

	delay(15000);


	if (!CheckConnection())
	{
		// Connection failed.
		WriteToScreen(_netFailed, _systemError);

		while (1) { delay(1); }
	}
}

void loop()
{
	int pressedButton = CheckButtonPress();

	switch (stage)
	{

	case _StandBy:
		// This is the default stage - this will lead to the "WaitingForReader" stage
		stage = _WaitingForReader;
		WriteToScreen(_welcome, _useTheReader);

		break;

	case _WaitingForReader:

		WaitingForReader();
		break;

	case _WaitingForButtonAfterUserRead:

		WaitingForButtonAfterUserRead(pressedButton);

		break;

	case _RegisteringUser:

		RegisterANewUser();
		break;


	default:
		break;

	}

	// Check if we need to go the Setup mode
	// Setup mode will let the admin to create
	// new user when the device is idle.

	if (stage == _WaitingForReader && pressedButton == _BothButton)
	{
		// Entering to the setupMode!
		stage = _RegisteringUser;
	}

	// Rest a little.
	delay(50);
}

void WaitingForReader()
{
	userID = getFingerprintID();

	if (userID > -1)
	{
		// We got a user data!

		// Gather the data from the server
		// We already displayed the "Success" screen
		// so the user know the process is working

		Serial.print("Found a fingerprint with ID ");
		Serial.println(userID);

		// Download the data to display:
		if (GetCheck(userID, data_Line0, data_Line1))
		{
			// Success!
			WriteToScreen(data_Line0, data_Line1);

			// Waiting for the RED or GREEN button
			stage = _WaitingForButtonAfterUserRead;
		}
		else
		{
			// Connection error happened!
			WriteToScreen(_netFailed, _systemError);

			// This count as a fatal error
			while (1) { delay(1); }
		}

		stage = _WaitingForButtonAfterUserRead;
	}
	else if (userID < -1)
	{
		// If error happened then the screen will display the message
		// and wait 2 seconds to return to this block.

		// We have to write out the regular message, as the system won't do it

		stage = _StandBy;
	}


	// If no data found it will return -1
	// and the process can continue regularly.

}

void WaitingForButtonAfterUserRead(int pressedButton) {

	// This method will run if the fingerprint reading
	// was successful. We need to wait if the user want to
	// log in, or log out from the system.

	int logResult = -1;

	switch (pressedButton)
	{
	case _RedButton:
		// User wish to log out.
		logResult = SendLogRequest(userID, 0);
		break;
	case _GreenButton:
		// User wish to log in.
		logResult = SendLogRequest(userID, 1);
		break;

	default:
		// No button was pressed
		// Wait till the user made up their mind.
		break;
	}

	if (logResult == 0)
	{
		// Error happened while trying to send the
		// request to the server.

		WriteToScreen(_netFailed, _tryAgain);
		delay(2000);

		stage = _StandBy;
	}
	else if (logResult == 1)
	{
		// Request was sent successfully!

		if (pressedButton == _RedButton)
		{
			WriteToScreen(_LoggedOut, _thankYou);
		}
		else
		{
			WriteToScreen(_LoggedIn, _thankYou);
		}

		delay(2000);

		stage = _StandBy;
	}
}

void RegisterANewUser()
{
	Serial.println("Entering setup mode!");

	WriteToScreen(_gettingID, _pleaseWait);

	userID = GetNewID();

	if (userID < 0)
	{
		WriteToScreen(_netFailed, _tryAgain);
		delay(2000);

		stage = _StandBy;

		return;
	}

	Serial.print("User id: ");
	Serial.println(userID);

	if (EnrollFinger(userID))
	{
		// Success!
		// Send the confirmation to the server!

		if (ConfirmBioID(userID))
		{
			// User is confirmed.
			WriteToScreen(_Success, _thankYou);
			delay(2000);
		}
		else
		{
			// Confirmation failed!!
			WriteToScreen(_netFailed, _ManualConfReq);
			delay(2000);
		}
	}
	else
	{
		// Failure!
		// User already got the message to try again.
	}

	stage = _StandBy;
}

int CheckButtonPress()
{
	buttonState = digitalRead(greenButtonPort);

	int returnVariable = _None;

	if (buttonState == HIGH) {
		// Green button is pressed!
		returnVariable = _GreenButton;
	}

	buttonState = digitalRead(redButtonPort);

	if (buttonState == HIGH) {
		// Red button is pressed!
		// Lets check if the green is pressed as well:

		if (returnVariable == _GreenButton)
		{
			returnVariable = _BothButton;
		}
		else
		{
			returnVariable = _RedButton;
		}

	}

	return returnVariable;
}