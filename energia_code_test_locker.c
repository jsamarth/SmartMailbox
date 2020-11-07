#include <Keypad.h>

// PIN ASSIGNMENTS
const int LED_PIN = GREEN_LED;
const int LOCK_PIN = 3;
const int SPEAKER_PIN = 31; // probably requires analogWrite
const int MEMBRANE_PIN_1 = 23;
const int MEMBRANE_PIN_2 = 24;
const int MEMBRANE_PIN_3 = 25;
const int MEMBRANE_PIN_4 = 26;
const int MEMBRANE_PIN_5 = 27;
const int MEMBRANE_PIN_6 = 28;
const int MEMBRANE_PIN_7 = 29;
const int MEMBRANE_PIN_8 = 30;

// MEMBRANE SET-UP
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};
//connect to the row pinouts of the keypad
byte rowPins[ROWS] = {MEMBRANE_PIN_8, MEMBRANE_PIN_7, MEMBRANE_PIN_6, MEMBRANE_PIN_5};
//connect to the column pinouts of the keypad
byte colPins[COLS] = {MEMBRANE_PIN_4, MEMBRANE_PIN_3, MEMBRANE_PIN_2, MEMBRANE_PIN_1}; 
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// STATE ASSIGNMENT
int type_count = 0;
char acceptable_codes[3][6] = {
	{'1','1','1','1','1','1'}, 
	{'1','2','3','4','5','6'}, 
	{'4','4','4','4','4','4'}
};
int timer = 100000;
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
}

void loop(){
	timer -= 1;

	// Time has elapsed to enter the code. State will be reset, lock will be shut
	if(timer < 0) {
		digitalWrite(LOCK_PIN, HIGH);
		timer = 100000;
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

	char customKey = customKeypad.getKey();

	if (customKey){
		Serial.println(customKey);

		// reset timer
		timer = 100000;

		// append character to the current array
		current_array[type_count] = customKey;
		type_count += 1;

		// If 6 characters are typed
		if(type_count == 6) {

			// If the code matches ...
			if(checkIfValid(current_array)) {

				// Unlock the box
				digitalWrite(LOCK_PIN, LOW);

				// Ring out the alarm for a second
				analogWrite(SPEAKER_PIN, 130);
				delay(1000);
				analogWrite(SPEAKER_PIN, 0);

				// Flash the LED for a second
				digitalWrite(LED_PIN, HIGH);
				delay(1000);
				digitalWrite(LED_PIN, LOW);
			}

			digitalWrite(LOCK_PIN, HIGH);
			timer = 100000;
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