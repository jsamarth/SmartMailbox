#include <Keypad.h>

// PIN ASSIGNMENTS
const int LED_PIN = 2;
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

  	// Initialize pins
	pinMode(LED_PIN, OUTPUT);
	pinMode(LOCK_PIN, OUTPUT);
	pinMode(SPEAKER_PIN, OUTPUT);

	pinMode(GREEN_LED, OUTPUT);
}

void loop(){
	Serial.println(5);
	digitalWrite(LED_PIN, HIGH);
	delay(1000);
	digitalWrite(LED_PIN, LOW);

	digitalWrite(GREEN_LED, HIGH);
	delay(1000);
	digitalWrite(GREEN_LED, LOW);

	char customKey = customKeypad.getKey();

	if (customKey){
		Serial.println(customKey);
	}
}