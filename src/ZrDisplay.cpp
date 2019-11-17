#include "ZrDebug.h"

#include "heltec.h"

#include "ZrComm.h"
#include "ZrDisplay.h"
#include "ZrGlobal.h"
#include "ZrNavigation.h"

void setupDisplay() {
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/,
               false /*Serial Enable*/);

  Heltec.display->setColor(WHITE);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->clear();
}

static const int BUFFER_SIZE = 50;
char display_buffer[BUFFER_SIZE];

// ArialMT_Plain_10
// ArialMT_Plain_16
// ArialMT_Plain_24

void refreshDisplay() {
  ZrFunction& f = navigation().currentFunction();

  Heltec.display->clear();

  snprintf(display_buffer, BUFFER_SIZE, "%s", xrIp().toString().c_str());

  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, display_buffer);

  snprintf(display_buffer, BUFFER_SIZE, "channel: %s",
           f.humanChannelName().c_str());
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(0, 10, display_buffer);

  snprintf(display_buffer, BUFFER_SIZE, "%s: %s", f.humanFunctionName().c_str(),
           f.cachedValue().asStrHuman().c_str());

  //   Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->drawString(0, 24, display_buffer);
  Heltec.display->display();
}