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

const byte ROWS = 4;  // four rows
const byte COLS = 4;  // four columns
// define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {{'1', '2', '3', 'A'},
                             {'4', '5', '6', 'B'},
                             {'7', '8', '9', 'C'},
                             {'E', '0', 'F', 'D'}};
// connect to the row pinouts of the keypad
byte rowPins[ROWS] = {27, 14, 12, 13};

// connect to the column pinouts of the keypad
byte colPins[COLS] = {32, 33, 25, 26};

// initialize an instance of class NewKeypad
Keypad customKeypad =
    Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup_heltecfac2() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/,
               false /*Serial Enable*/);

  logo();
  delay(1000);
  Heltec.display->clear();
  for (int i = 0; i < 80; i += 20) {
    for (int j = 0; j < 80; j += 20) {
      Serial.print(i);
      Serial.print(":");
      Serial.println(j);
      Heltec.display->clear();
      Heltec.display->drawString(i, j, "Foobar");
      Heltec.display->display();
      delay(500);
    }
  }

  // Heltec.display->clear();

  // WIFISetUp();
}

void setup_mykeypad() {
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/,
               false /*Serial Enable*/);

  Heltec.display->setColor(WHITE);

  Heltec.display->clear();
  // Heltec.display->init();
  // This does not work
  // Heltec.display->flipScreenVertically();
}

void loop_mykeypad() {

  char customKey = customKeypad.getKey();

  if (customKey) {
    // Heltec.display->clear();
    Heltec.display->drawString(0, 0, "Barbar: 50\nFoobar: 10\nFoo");
    Heltec.display->display();
    // delay(1000);

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
      case 'E':
        break;
      case 'F':
        break;
      case 'D':
        break;
      case '0':
        Heltec.display->clear();
        break;
      default:
        break;
    }
    delay(300);
  }
}
