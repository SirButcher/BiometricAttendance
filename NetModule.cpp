// This module will handle the internet connection toward
// the server.
// The server will require a HTTPS connection, on port 443

#include "NetModule.h"
#include <SPI.h>
#include <Ethernet.h>

IPAddress serverIP(78, 109, 163, 153);
char server[] = "phone.sipcarparks.co.uk";
int port = 80;



// Static details in case the DHCP doesn't give us an address
IPAddress ip(192, 168, 0, 177);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress myDns(8, 8, 8, 8); // google public dns

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
#if defined(WIZ550io_WITH_MACADDRESS) // Use assigned MAC address of WIZ550io
;
#else
byte mac[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x03, 0x8D }; // From the sticker
#endif

char address[48] = "/Entry/BioEntry.ashx?PSW=z3He8mMKwSw7yGch&TYPE=";
char testAddress[18] = "/Entry/Check.ashx";
char bioID[8] = "&BIOID=";
char connectionVersion[10] = " HTTP/1.1";


char newUserLink[4] = "NEW";
char confirmLink[8] = "NEWDONE";
char checkLink[6] = "CHECK";	// Last char needs to be replaced with the ID! - Char 60
char loginLink[6] = "LOGIN";	// Last char needs to be replaced with the ID! - Char 60
char logoutLink[7] = "LOGOUT";  // Last char needs to be replaced with the ID! - Char 61

EthernetClient client;

void SetupNetModule()
{
	Serial.println("Initializing net module, please wait..");

	// initialize the ethernet device
#if defined __USE_DHCP__
#if defined(WIZ550io_WITH_MACADDRESS) // Use assigned MAC address of WIZ550io
	Ethernet.begin();
#else
	Ethernet.begin(mac);
#endif  
#else
#if defined(WIZ550io_WITH_MACADDRESS) // Use assigned MAC address of WIZ550io
	Ethernet.begin(ip, myDns, gateway, subnet);
#else
	Ethernet.begin(mac, ip, myDns, gateway, subnet);
#endif  
#endif 

	Serial.println("Net module is ready!");
}

int GetNewID()
{
	return 0;
}

void GetCheck(int, char *)
{
}

void Login(int)
{
}

void Logout(int)
{
}

int CheckConnection()
{
	if (client.connect(serverIP, port))
	{
		// For a successful connection
		// We have to build up the HTML package.
		// It is going to look like this:
		// GET /WEBPAGE.HTML HTTP/1.1
		// Host: DOMAIN
		// Connection: close

		// Sending the above will result in a successful
		// connection. Hopefully.


		Serial.println("Client is Connected");
		// Make a HTTP request:
		client.print("GET ");
		client.print(testAddress);

		client.println(connectionVersion);
		client.print("Host: ");
		client.println(server);

		client.println("Connection: close");
		client.println();

		// Read the data while it is available
		int hasData = 1;

		do 
		{
			// if there are incoming bytes available
			// from the server, read them and print them:
			if (client.available()) {

				Serial.println("Data arrived!");
				client.stop();

				// Exit from the loop
				hasData = 0;
			}

			// if the server's disconnected, stop the client:
			if (!client.connected()) {
				Serial.println();
				Serial.println("disconnecting.");
				client.stop();

				hasData = 0;
			}

			delayMicroseconds(50);

		} while (hasData);

		Serial.println("Connection is working!");

		return 1;
	}
	else 
	{
		// If the connection failed:
		Serial.println("connection failed");

		return 0;
	}

	// In case I mess up something in the future!
	return 0;
}