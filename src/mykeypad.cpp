/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/

#include <ArduinoTrace.h>
#include <Keypad.h>
#include <heltec.h>
// #include <oled/OLEDDisplay.h>

#include "mykeypad.h"

const byte ROWS = 4;  // four rows
const byte COLS = 4;  // four columns
// define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {{'1', '2', '3', 'A'},
                             {'4', '5', '6', 'B'},
                             {'7', '8', '9', 'C'},
                             {'E', '0', 'F', 'D'}};
// byte rowPins[ROWS] = {3, 2, 1, 0}; //connect to the row pinouts of the keypad
// byte colPins[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the
// keypad
byte rowPins[ROWS] = {23, 19, 22,
                      21};  // connect to the row pinouts of the keypad
byte colPins[COLS] = {16, 17, 5,
                      18};  // connect to the column pinouts of the keypad

// initialize an instance of class NewKeypad
Keypad customKeypad =
    Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup_mykeypad() {
  // TODO: not needed?
	pinMode(LED,OUTPUT);
	digitalWrite(LED,HIGH);

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/,
               true /*Serial Enable*/);

  Heltec.display->clear();
}

void loop_mykeypad() {
  char customKey = customKeypad.getKey();

  if (customKey) {
    Serial.println(customKey);
    switch (customKey) {
      case 'A':
        Heltec.display->setFont(ArialMT_Plain_10);
        break;
      case 'B':
        Heltec.display->setFont(ArialMT_Plain_16);
        break;
      case 'C':
        Heltec.display->setFont(ArialMT_Plain_24);
        break;
    }
    Heltec.display->drawString(0, 0, "Foobar");
    Heltec.display->display();
  }
}
