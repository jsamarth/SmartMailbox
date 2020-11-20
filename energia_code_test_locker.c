#include "assignments.h"

void setup() {
	//Initialize serial and wait for port to open:
	Serial.begin(115200);

	pinMode(YELLOW_LED_PIN, OUTPUT);
	pinMode(GREEN_LED_PIN, OUTPUT);
	pinMode(BLUE_LED_PIN, OUTPUT);

	pinMode(SPEAKER_PIN, OUTPUT);
	pinMode(LOCK_PIN, OUTPUT);

	for(int i=0;i<4;i++)
	{
		pinMode(rpin[i],OUTPUT);
		pinMode(cpin[i],INPUT_PULLDOWN);
	}

	curr_state = RST_INPUT;

	// attempt to connect to Wifi network:
	Serial.print("Attempting to connect to Network named: ");
	digitalWrite(BLUE_LED_PIN, HIGH);
	// print the network name (SSID);
	Serial.println(ssid); 
	// Connect to WPA/WPA2 network. Change this line if using open or WEP network:
	WiFi.begin(ssid, password);
	while ( WiFi.status() != WL_CONNECTED) {
	// print dots while we wait to connect
		Serial.print(".");
		delay(300);

		// digitalWrite(BLUE_LED_PIN, HIGH);
		// delay(200);
		// digitalWrite(BLUE_LED_PIN, LOW);
	}

	digitalWrite(BLUE_LED_PIN, LOW);

	Serial.println("\nYou're connected to the network");
	Serial.println("Waiting for an ip address");

	while (WiFi.localIP() == INADDR_NONE) {
		// print dots while we wait for an ip addresss
		Serial.print(".");
		delay(300);
		// digitalWrite()
	}

	Serial.println("\nIP Address obtained");

	// you're connected now, so print out the status:
	printWifiStatus();

	// start the server:
	server.begin();
}

void loop() {
	// wait for a new client:
	// digitalWrite(YELLOW_LED_PIN, LOW);
	// digitalWrite(GREEN_LED_PIN, LOW);
	WiFiClient client = server.available();

	// when the client sends the first byte, say hello:
	if (client) {
		digitalWrite(BLUE_LED_PIN, HIGH);
		// digitalWrite(YELLOW_LED_PIN, LOW);
		// digitalWrite(GREEN_LED_PIN, LOW);
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
			
			// echo the bytes to the server as well:
			// Serial.println(clientInp);

			if(clientInp.substring(0,2) == "ND") {
				Serial.println("adding " + clientInp.substring(3,9));
				acceptable_codes.push_back(clientInp.substring(3,9));
			} 

			if(clientInp.substring(0,4) == "OPEN") {
				Serial.println("Open command from app.");
				curr_state = CORRECT_CODE;
			} 
		}

		if(curr_state == RST_INPUT) {
			timer = TIMER_MAX;
			current_string = "";
			type_count = 0;

			// lock enabled
			digitalWrite(LOCK_PIN, LOW);
			
			digitalWrite(YELLOW_LED_PIN, HIGH);

			digitalWrite(GREEN_LED_PIN, LOW);

			char c = getKey();
			if(c) {
				delay(BUTTON_DEBOUNCE_TIME);

				// Beep to indicate button click
				flash(SPEAKER_PIN, 1);

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

			digitalWrite(YELLOW_LED_PIN, LOW);

			if(timer < 0) {
				Serial.println("Resetting!");
				curr_state = RST_INPUT;

				flash(YELLOW_LED_PIN, 3);
			}

			if(type_count == 6) {
				if(checkIfValid()) {
					Serial.println("Correct code!");
					curr_state = CORRECT_CODE;
					client.println("UNLCKD " + current_string);
					findAndRemoveCurrentString();
				}
				else {
					curr_state = WRONG_CODE;
				}
			} 

			else {
				char c = getKey();
				if(c) {
					delay(BUTTON_DEBOUNCE_TIME);

					// Beep to indicate button click
					flash(SPEAKER_PIN, 1);

					Serial.print(c);
					Serial.println(" was typed");

					current_string += c;
					type_count += 1;
					printCurrArray();
				}
			}

		}

		else if(curr_state == CORRECT_CODE) {

			digitalWrite(LOCK_PIN, HIGH);

			flash(SPEAKER_PIN, 1);

			digitalWrite(GREEN_LED_PIN, HIGH);

			char c = getKey();
			if(c=='#') {
				delay(BUTTON_DEBOUNCE_TIME);

				Serial.println("Locking again!");
				curr_state = RST_INPUT;
				client.println("LOCKED");
				flash(YELLOW_LED_PIN, 3);
			}
		}

		else if(curr_state == WRONG_CODE) {
			flash(YELLOW_LED_PIN, 3);

			Serial.println("Wrong code!");
			client.println("WRNGCD " + current_string);
			curr_state = RST_INPUT;
		}
	}
	else {
		digitalWrite(BLUE_LED_PIN, LOW);
		digitalWrite(GREEN_LED_PIN, LOW);
		digitalWrite(YELLOW_LED_PIN, LOW);
		if (alreadyConnected) Serial.write("Client disconnected!\n");
		alreadyConnected = false;

	}
}