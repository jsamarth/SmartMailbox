#include "assignments.h"

void setup() {
	//Initialize serial and wait for port to open:
	Serial.begin(115200);

	pinMode(RED_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);

	for(int i=0;i<4;i++)
	{
		pinMode(rpin[i],OUTPUT);
		pinMode(cpin[i],INPUT_PULLDOWN);
	}

	curr_state = RST_INPUT;

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
	// wait for a new client:
	WiFiClient client = server.available();

	// when the client sends the first byte, say hello:
	if (client) {
		digitalWrite(GREEN_LED, HIGH);
		// Serial.println(curr_state);

		// When the client connects for the first time
		if (!alreadyConnected) {
			// clead out the input buffer:
			client.flush();
			Serial.println("We have a new client");
			client.println("Hello, client!");
			alreadyConnected = true;
		}

		if(client.available() > 0) {
			// read the bytes incoming from the client:
			// char thisChar = client.read();
			String clientInp = "";
			while (client.available() > 0)
				clientInp += (char)client.read();
			// echo the bytes back to the client:
			server.println(clientInp);
			// echo the bytes to the server as well:
			// Serial.println(clientInp);
		}

		if(curr_state == RST_INPUT) {
			timer = TIMER_MAX;
			// for(int i = 0; i < 6; i++)
				// current_string[i] = '\0';
			current_string = "";
			type_count = 0;

			// lock enabled
			digitalWrite(LOCK_PIN, LOW);

			char c = getKey();
			if(c) {
				delay(BUTTON_DEBOUNCE_TIME);
				Serial.print(c);
				Serial.println(" was typed");

				curr_state = ACCPT_CODE;
				type_count += 1;
				current_string = String(c);
				printCurrArray();
			}
		}

		else if(curr_state == ACCPT_CODE) {
			timer -= 1;

			if(timer < 0) {
				Serial.println("Resetting!");
				curr_state = RST_INPUT;
			}

			if(type_count == 6) {
				if(checkIfValid()) {
					Serial.println("Correct code!");
					curr_state = CORRECT_CODE;
					client.println("UNLOCKED " + current_string);
				}
				else {
					curr_state = WRONG_CODE;
				}
			} 

			else {
				char c = getKey();
				if(c) {
					delay(BUTTON_DEBOUNCE_TIME);
					Serial.print(c);
					Serial.println(" was typed");

					current_string += c;
					type_count += 1;
					printCurrArray();
				}
			}

		}

		else if(curr_state == CORRECT_CODE) {
			char c = getKey();
			if(c=='#') {
				delay(BUTTON_DEBOUNCE_TIME);
				Serial.println("Locking again!");
				curr_state = RST_INPUT;
				client.println("LOCKED");
				// Locking again
			}
		}

		else if(curr_state == WRONG_CODE) {
			Serial.println("Wrong code!");
			client.println("WRONG_CODE " + current_string);
			curr_state = RST_INPUT;
		}
	}
	else {
		digitalWrite(RED_LED, LOW);
		if (alreadyConnected) Serial.write("Client disconnected!\n");
		alreadyConnected = false;

	}
}