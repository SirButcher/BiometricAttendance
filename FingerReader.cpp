// This library must used to enroll users to the fingerprint reader's memory.
// Green cable  - Pin 2
// White cable  - Pin 3

#include "FingerReader.h"
#include "LcdScreen.h"

#include "App_Constants.h"

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>


SoftwareSerial mySerial(4, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int status = -1;

char converted[3] = "  ";



void SetupReader()
{
	Serial.println("Initializing fingerprint reader!");

	finger.begin(57600);

	if (finger.verifyPassword()) {
		// Sensor found.
	}
	else {
		// Sensor is missing!

		WriteToScreen(_systemError, _EmptyLine);

		while (1) { delay(1); }
	}
}

// Return 0 if fails
//		  1 if success
int EnrollFinger(uint8_t bioID)
{
	status = -1;

	// First enroll round.
	// We have to wait until we get back a "OK" 
	// or user break the process (pressing the red button)

	while (status != FINGERPRINT_OK) {

		status = finger.getImage();

		switch (status) {

		case FINGERPRINT_OK:
			// Successfully took an image
			break;

		case FINGERPRINT_NOFINGER:
			// Still waiting. Check the red button
			break;

		case FINGERPRINT_PACKETRECIEVEERR:
			WriteToScreen(_systemError, 0);
			while (1) { delay(1); }

			break;

		case FINGERPRINT_IMAGEFAIL:
			WriteToScreen(_imageError, _tryAgain);

			break;
		default:
			WriteToScreen(_systemError, _tryAgain);

			break;
		}
	}

	// OK success!
	// Convert the image.

	status = finger.image2Tz(1);

	switch (status) {
	case FINGERPRINT_OK:
		// Image converted!
		// We can go to the next step
		break;
	default:
		// Error happened
		WriteToScreen(_imageError, _tryAgain);
		delay(2000);
		return -1;
	}

	WriteToScreen(_imageTook, _removeFinger);

	// Image is converted or we try again.
	// Now we are waiting for the user to remove 
	// their finger.

	status = 0;
	delay(5000);

	while (status != FINGERPRINT_NOFINGER) {
		status = finger.getImage();
	}

	// Finger is removed!
	// Confirming the read print.

	WriteToScreen(_enroll0, _enroll1);


	status = -1;

	while (status != FINGERPRINT_OK) {

		status = finger.getImage();

		switch (status) {

		case FINGERPRINT_OK:
			// OK
			break;
		case FINGERPRINT_NOFINGER:
			// Still waiting. Check the red button
			break;
		case FINGERPRINT_PACKETRECIEVEERR:
			WriteToScreen(_systemError, 0);
			while (1) { delay(1); }
			break;
		case FINGERPRINT_IMAGEFAIL:
			WriteToScreen(_imageError, _tryAgain);
			break;
		default:
			WriteToScreen(_imageError, _tryAgain);
			break;
		}
	}

	// Image for the confirmation is took!
	// Process it again as we did previously.

	status = finger.image2Tz(2);

	switch (status) {
	case FINGERPRINT_OK:
		// OK
		break;
	default:
		WriteToScreen(_imageError, _tryAgain);
		delay(2000);
		return 0;
	}

	// Second image is converted successfully!
	// Time to create the model
	// and store it under the designated BIO id.

	status = finger.createModel();
	if (status == FINGERPRINT_OK) {
		// Huzzah!
	}
	else if (status == FINGERPRINT_ENROLLMISMATCH) {
		WriteToScreen(_imageNotMatch, _tryAgain);
		delay(2000);
		return 0;
	}
	else {
		WriteToScreen(_systemError, _tryAgain);
		delay(2000);
		return 0;
	}

	// Okay, so far so good!
	// If user reached this point
	// that means they was able to 
	// use the same finger twice!
	// And the system recorded it. 

	// Time to store the data!

	status = finger.storeModel(bioID);

	if (status == FINGERPRINT_OK) {
		// Data stored! Hurray!
	}
	else {
		WriteToScreen(_imageError, _tryAgain);
		delay(2000);
		return 0;
	}
	
	status = -1;

	// Image stored! Creating the message now

	itoa(bioID, converted, 10);

	_userStored1[12] = ' ';
	_userStored1[13] = ' ';
	_userStored1[14] = ' ';

	if (bioID < 10)
	{
		_userStored1[12] = converted[0];
	}
	else if (bioID < 100)
	{
		_userStored1[12] = converted[0];
		_userStored1[13] = converted[1];
	}
	else
	{
		_userStored1[12] = converted[0];
		_userStored1[13] = converted[1];
		_userStored1[14] = converted[2];
	}

	WriteToScreen(_userStored0, _userStored1);

	delay(5000);

	return 1;
}

// User found: >= 0
// No data: -1
// Error: -2
// In case of the error screen will display the error
// and will wait 2 seconds
int getFingerprintID()
{
	status = -1;

	status = finger.getImage();

	switch (status) {

	case FINGERPRINT_OK:
		Serial.println("Image taken");
		break;
	case FINGERPRINT_NOFINGER:
		return -1;
	default:
		WriteToScreen(_imageError, _tryAgain);
		delay(2000);

		return -2;

	}

	// OK success!
	status = finger.image2Tz();

	if (status != FINGERPRINT_OK) {
		// Error happened during the conversion

		WriteToScreen(_imageError, _tryAgain);
		delay(2000);

		return -2;
	}

	// OK converted!
	status = finger.fingerFastSearch();

	switch (status)
	{

	case FINGERPRINT_OK:
		// found a match!
		Serial.print("Found ID #"); Serial.print(finger.fingerID);
		Serial.print(" with confidence of "); Serial.println(finger.confidence);

		WriteToScreen(_imageTook, _pleaseWait);

		return finger.fingerID;

	case FINGERPRINT_NOTFOUND:

		WriteToScreen(_imageError, _userNotFound);

		delay(2000);
		return -1;

	default:
		// We can skip the different error types
		// as for us all of them mean one thing:
		// The operation is failed.
		WriteToScreen(_imageError, _tryAgain);

		delay(2000);
		return -2;

	}

}
