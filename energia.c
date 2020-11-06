#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include <WiFi.h>

// your network name also called SSID
char ssid[] = "wifi-sj";
// your network password
char password[] = "fenderbender10";

int count = 1000;

WiFiServer server(23);

boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {
	//Initialize serial and wait for port to open:
	Serial.begin(115200);

	pinMode(RED_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);

	// attempt to connect to Wifi network:
	Serial.print("Attempting to connect to Network named: ");
	// print the network name (SSID);
	Serial.println(ssid); 
	// Connect to WPA/WPA2 network. Change this line if using open or WEP network:
	WiFi.begin(ssid, password);
	while ( WiFi.status() != WL_CONNECTED) {
	// print dots while we wait to connect
		Serial.print(".");
		delay(300);
	}

	Serial.println("\nYou're connected to the network");
	Serial.println("Waiting for an ip address");

	while (WiFi.localIP() == INADDR_NONE) {
		// print dots while we wait for an ip addresss
		Serial.print(".");
		delay(300);
	}

	Serial.println("\nIP Address obtained");

	// you're connected now, so print out the status:
	printWifiStatus();

	// start the server:
	server.begin();
}

void loop() {

	count -= 1;
	// wait for a new client:
	WiFiClient client = server.available();

	// when the client sends the first byte, say hello:
	if (client) {
		digitalWrite(GREEN_LED, HIGH);

		// When the client connects for the first time
		if (!alreadyConnected) {
			// clead out the input buffer:
			client.flush();
			Serial.println("We have a new client");
			client.println("Hello, client!");
			alreadyConnected = true;
		}

		if(count < 0) {
			count = 1000;
			digitalWrite(RED_LED, HIGH);
			delay(300);
			digitalWrite(RED_LED, LOW);
			client.println("ping!");
		}

		if (client.available() > 0) {
			// read the bytes incoming from the client:
			char thisChar = client.read();
			// echo the bytes back to the client:
			server.write(thisChar);
			// echo the bytes to the server as well:
			Serial.write(thisChar);
		}
	}
	else {
		digitalWrite(RED_LED, LOW);
		if (alreadyConnected) Serial.write("Client disconnected!\n");
		alreadyConnected = false;

	}
}

void printWifiStatus() {
	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());

	// print your WiFi shield's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);

	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.print(rssi);
	Serial.println(" dBm");
}

