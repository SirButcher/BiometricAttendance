// This library must used to enroll users to the fingerprint reader's memory.
// Green cable  - Pin 2
// White cable  - Pin 3

#include "FingerReader.h"
#include "LcdScreen.h"

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>


SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int status = -1;

char converted[3] = "  ";

char unknowError[32] =		"  System error!  Please restart";
char imgError[32] =			"   Image error!    Try again   ";
char imgMsmMathc[32] =		" Image not match    Try again  ";
char notFound[32] =			" User not found    Try again   ";

char imageTook[32] =		"   Image taken   remove finger ";
char enrollConfirm[32] =	" Put same finger on the reader ";
char userStored[32] =		"  Success! User  stored. ID:   ";

void SetupReader()
{
	Serial.println("Initializing fingerprint reader!");

	finger.begin(57600);

	if (finger.verifyPassword()) {
		// Sensor found.

		finger.getTemplateCount();
		Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
	}
	else {
		// Sensor is missing!

		WriteToScreen(&unknowError[0]);

		while (1) { delay(1); }
	}
}

void EnrollFinger(uint8_t bioID)
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
			WriteToScreen(&unknowError[0]);
			while (1) { delay(1); }
			break;
		case FINGERPRINT_IMAGEFAIL:
			WriteToScreen(&imgError[0]);
			break;
		default:
			WriteToScreen(&unknowError[0]);
			break;
		}
	}


	// OK success!
	// Convert the image.

	status = finger.image2Tz(1);

	switch (status) {
	case FINGERPRINT_OK:
		// Image converted!
		break;
	case FINGERPRINT_IMAGEMESS:
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	case FINGERPRINT_PACKETRECIEVEERR:
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	case FINGERPRINT_FEATUREFAIL:
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	case FINGERPRINT_INVALIDIMAGE:
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	default:
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	}

	WriteToScreen(&imageTook[0]);

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

	WriteToScreen(&enrollConfirm[0]);
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
			WriteToScreen(&unknowError[0]);
			while (1) { delay(1); }
			break;
		case FINGERPRINT_IMAGEFAIL:
			WriteToScreen(&imgError[0]);
			break;
		default:
			WriteToScreen(&unknowError[0]);
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
	case FINGERPRINT_IMAGEMESS:
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	case FINGERPRINT_PACKETRECIEVEERR:
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	case FINGERPRINT_FEATUREFAIL:
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	case FINGERPRINT_INVALIDIMAGE:
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	default:
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	}

	// Second image is converted successfully!
	// Time to create the model
	// and store it under the designated BIO id.

	status = finger.createModel();
	if (status == FINGERPRINT_OK) {
		// Huzzah!
	}
	else if (status == FINGERPRINT_PACKETRECIEVEERR) {
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	}
	else if (status == FINGERPRINT_ENROLLMISMATCH) {
		WriteToScreen(&imgMsmMathc[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	}
	else {
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
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
	else if (status == FINGERPRINT_PACKETRECIEVEERR) {
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	}
	else if (status == FINGERPRINT_BADLOCATION) {
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	}
	else if (status == FINGERPRINT_FLASHERR) {
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	}
	else {
		WriteToScreen(&imgError[0]);
		delay(2000);
		EnrollFinger(bioID);
		return;
	}
	
	status = -1;

	// Image stored! Creating the message now

	itoa(bioID, converted, 10);

	if (bioID < 10)
	{
		userStored[29] = converted[0];
		userStored[30] = ' ';
		userStored[31] = ' ';
	}
	else if (bioID < 100)
	{
		userStored[29] = converted[0];
		userStored[30] = converted[1];
		userStored[31] = ' ';
	}
	else
	{
		userStored[29] = converted[0];
		userStored[30] = converted[1];
		userStored[31] = converted[2];
	}

	WriteToScreen(&userStored[0]);
	delay(5000);
}

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
	case FINGERPRINT_PACKETRECIEVEERR:
		WriteToScreen(&imgError[0]);
		delay(2000);
		return -2;
	case FINGERPRINT_IMAGEFAIL:
		WriteToScreen(&imgError[0]);
		delay(2000);
		return -2;
	default:
		WriteToScreen(&imgError[0]);
		delay(2000);
		return -2;
	}

	// OK success!

	status = finger.image2Tz();

	if (status != FINGERPRINT_OK) {
		// Error happened during the conversion

		WriteToScreen(&imgError[0]);
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
		return finger.fingerID;

	case FINGERPRINT_NOTFOUND:
		WriteToScreen(&notFound[0]);
		delay(2000);
		return -1;

	default:
		// We can skip the different error types
		// as for us all of them mean one thing:
		// The operation is failed.
		WriteToScreen(&imgError[0]);
		delay(2000);
		return -2;
	}

}
