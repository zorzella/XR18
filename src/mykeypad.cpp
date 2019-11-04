/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/

#include "heltec.h"

#include <ArduinoTrace.h>
#include <Keypad.h>
// #include <oled/OLEDDisplay.h>

#include "heltecfac.h"
#include "mykeypad.h"
#include "ZrKeypad.h"

// const byte ROWS = 4;  // four rows
// const byte COLS = 4;  // four columns
// // define the cymbols on the buttons of the keypads
// char hexaKeys[ROWS][COLS] = {{'1', '2', '3', 'A'},
//                              {'4', '5', '6', 'B'},
//                              {'7', '8', '9', 'C'},
//                              {'E', '0', 'F', 'D'}};
// // // connect to the row pinouts of the keypad
// // byte rowPins[ROWS] = {27, 14, 12, 13};

// // // connect to the column pinouts of the keypad
// // byte colPins[COLS] = {32, 33, 25, 26};

// // connect to the row pinouts of the keypad
// byte rowPins[ROWS] = {23, 19, 22, 21};

// // connect to the column pinouts of the keypad
// byte colPins[COLS] = {2, 17, 5, 18};

// // initialize an instance of class NewKeypad
// Keypad customKeypad =
//     Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup_mykeypad() {
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/,
               false /*Serial Enable*/);

  Heltec.display->setColor(WHITE);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->clear();
}

char buffer[50];

void loop_mykeypad() {
  char customKey = zrKeypad.getKey();

  if (customKey) {

    snprintf(buffer, 50, "Key: %c", customKey);
    
    Serial.println(buffer);

    Heltec.display->clear();

    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(0, 0, buffer);

    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(0, 10, buffer);
    
    Heltec.display->setFont(ArialMT_Plain_24);
    Heltec.display->drawString(0, 24, buffer);
    Heltec.display->display();
    // delay(1000);

    Serial.println(customKey);
    switch (customKey) {
      case 'A':
        // Heltec.display->setFont(ArialMT_Plain_10);
        break;
      case 'B':
        // Heltec.display->setFont(ArialMT_Plain_16);
        break;
      case 'C':
        // Heltec.display->setFont(ArialMT_Plain_24);
        break;
      case 'E':
        break;
      case 'F':
        break;
      case 'D':
        break;
      case '0':
        break;
      default:
        break;
    }
    // delay(300);
  }
}
