#ifndef __CC3200R1M1RGC__
#include <SPI.h>
#endif
#include <WiFi.h>
#include <vector>

#define BUTTON_DEBOUNCE_TIME 1000

// FSM STATES
#define START 0
#define RST_INPUT 1
#define ACCPT_CODE 2
#define CORRECT_CODE 3
#define WRONG_CODE 4

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
// char acceptable_codes[3][6] = {
//   {'1','1','1','1','1','1'}, 
//   {'1','2','3','4','5','6'}, 
//   {'4','4','4','4','4','4'}
// };
// std::vector<String> acceptable_codes(1, "111111");
std::vector<String> acceptable_codes();

const int TIMER_MAX = 20000;
int timer = TIMER_MAX;
String current_string = String(6);
int curr_state = RST_INPUT;

boolean checkIfValid() {
  for(int i = 0; i < acceptable_codes.size(); i++) {
    for(int j = 0; j < 6; j++) {
      if(current_string[j] != acceptable_codes[i][j])
        break;
      if(j == 5) {
        // Serial.print(i);
        // Serial.println(" is the correct code");
        return true;
      }
    }
  }

  return false;
}

char getKey() {
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

void findAndRemoveCurrentString() {
  for (std::vector<String>::iterator it = acceptable_codes.begin() ; it != acceptable_codes.end(); ++it) {
    if(current_string.equals(*it)) {
      acceptable_codes.erase(it);
      break;
    }
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

void printCurrArray() {
  Serial.println("Current array ... ");
  Serial.println(current_string);
  Serial.println();
}
