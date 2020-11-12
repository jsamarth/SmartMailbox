#ifndef __CC3200R1M1RGC__
#include <SPI.h>
#endif
#include <WiFi.h>

// WIFI SETUP
char ssid[] = "wifi-sj";
char password[] = "fenderbender10";
WiFiServer server(23);
boolean alreadyConnected = false; // whether or not the client was connected previously

// PIN ASSIGNMENTS
const int LED_PIN = GREEN_LED;
const int LOCK_PIN = 3;
const int SPEAKER_PIN = 31; // probably requires analogWrite
const int MEMBRANE_PIN_1 = 2;
const int MEMBRANE_PIN_2 = 3;
const int MEMBRANE_PIN_3 = 4;
const int MEMBRANE_PIN_4 = 5;
const int MEMBRANE_PIN_5 = 6;
const int MEMBRANE_PIN_6 = 7;
const int MEMBRANE_PIN_7 = 8;
const int MEMBRANE_PIN_8 = 9;

// MEMBRANE SET-UP
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char keymap[ROWS][COLS] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};
//connect to the row pinouts of the keypad
byte rpin[ROWS] = {MEMBRANE_PIN_8, MEMBRANE_PIN_7, MEMBRANE_PIN_6, MEMBRANE_PIN_5};
//connect to the column pinouts of the keypad
byte cpin[COLS] = {MEMBRANE_PIN_4, MEMBRANE_PIN_3, MEMBRANE_PIN_2, MEMBRANE_PIN_1}; 
//initialize an instance of class NewKeypad

// STATE ASSIGNMENT
int type_count = 0;
char acceptable_codes[3][6] = {
	{'1','1','1','1','1','1'}, 
	{'1','2','3','4','5','6'}, 
	{'4','4','4','4','4','4'}
};
const int TIMER_MAX = 100000000;
int timer = TIMER_MAX;
char current_array[6];

boolean checkIfValid(char array[]) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 6; j++) {
			if(array[j] != acceptable_codes[i][j])
				continue;
			if(j == 5)
				return true;
		}
	}

	return false;
}

char getKey()
{
	char key=0;
	for(int i=0;i<4;i++)
	{
		digitalWrite(rpin[i],HIGH);
		for(int j=0;j<4;j++)
		{
			if (digitalRead(cpin[j])==HIGH)
				key=keymap[i][j];
		}
		digitalWrite(rpin[i],LOW);
	}
	return key;
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