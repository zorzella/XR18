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
  ZrNavigation zrNavigation = ZrNavigation::instance();
  ZrFunction& currentFunction = zrNavigation.currentFunction();

  ZrComm zrComm = ZrComm::instance();

  Heltec.display->clear();

  if (zrComm.isConnectedToNetwork()) {
    if (zrComm.isConnectedToXr()) {
      snprintf(display_buffer, BUFFER_SIZE, "%s - %s", zrComm.networkName(),
               zrComm.xrName());
    } else {
      snprintf(display_buffer, BUFFER_SIZE, "%s - finding an XR",
               zrComm.networkName());
    }
  } else {
    snprintf(display_buffer, BUFFER_SIZE, "Trying to connect to %s",
             zrComm.networkName());
  }

  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, display_buffer);

  snprintf(display_buffer, BUFFER_SIZE, "(%i) %s",
           zrNavigation.getCurrentPageIndex(),
           currentFunction.humanChannelName().c_str());
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(0, 10, display_buffer);

  snprintf(display_buffer, BUFFER_SIZE, "%s: %s",
           currentFunction.humanFunctionName().c_str(),
           currentFunction.cachedValue().asStrHuman().c_str());

  //   Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->drawString(0, 24, display_buffer);
  Heltec.display->display();
}