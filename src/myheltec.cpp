#include "myheltec.h"

#include "heltec.h"

void doHeltec() {
  Heltec.display->drawString(0, 0, "Connecting...");
  Heltec.display->display();
}
