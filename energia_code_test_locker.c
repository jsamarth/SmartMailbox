#include "assignments.h"

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