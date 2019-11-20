/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include "ZrDebug.h"

#include "heltec.h"

#include <Keypad.h>

#include "ZrGlobal.h"
#include "ZrKeypad.h"
#include "heltecfac.h"
#include "mykeypad.h"

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
