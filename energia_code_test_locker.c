#include <Keypad.h>

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

void setup(){
	Serial.begin(115200);
	Serial.println("Starting ...");

  	// Initialize pins
	pinMode(LED_PIN, OUTPUT);
	pinMode(LOCK_PIN, OUTPUT);
	pinMode(SPEAKER_PIN, OUTPUT);

	pinMode(GREEN_LED, OUTPUT);

	// Shut the lock
	digitalWrite(LOCK_PIN, HIGH);

	for(int i=0;i<4;i++)
	{
		pinMode(rpin[i],OUTPUT);
		pinMode(cpin[i],INPUT_PULLDOWN);
	}
}

void loop(){
	timer -= 1;

	// Time has elapsed to enter the code. State will be reset, lock will be shut
	if(timer < 0) {
		Serial.println("Resetting timer ...");
		digitalWrite(LOCK_PIN, HIGH);
		timer = TIMER_MAX;
		for(int c = 0; c < 6; c++)
			current_array[c] = '\0';
		type_count = 0;
	}

	// Serial.println(5);
	// digitalWrite(LED_PIN, HIGH);
	// delay(1000);
	// digitalWrite(LED_PIN, LOW);

	// digitalWrite(GREEN_LED, HIGH);
	// delay(1000);
	// digitalWrite(GREEN_LED, LOW);

	char customKey = getKey();

	if (customKey){
		Serial.println(customKey);
		delay(1000);

		// reset timer
		timer = TIMER_MAX;

		// append character to the current array
		current_array[type_count] = customKey;
		type_count += 1;

		Serial.println("Current code: ");
		for(int c = 0; c < 6; c++)
			Serial.print(current_array[c]);
		Serial.println();

		// If 6 characters are typed
		if(type_count == 6) {

			// If the code matches ...
			if(checkIfValid(current_array)) {

				Serial.println("Correct Code!");

				// Unlock the box
				digitalWrite(LOCK_PIN, LOW);

				// // Ring out the alarm for a second
				// analogWrite(SPEAKER_PIN, 130);
				// delay(1000);
				// analogWrite(SPEAKER_PIN, 0);

				// // Flash the LED for a second
				// digitalWrite(LED_PIN, HIGH);
				// delay(1000);
				// digitalWrite(LED_PIN, LOW);
			}

			else {
				Serial.println("Wrong Code!");
			}

			digitalWrite(LOCK_PIN, HIGH);
			timer = TIMER_MAX;
			for(int c = 0; c < 6; c++)
				current_array[c] = '\0';
			type_count = 0;
		}
	}
}

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