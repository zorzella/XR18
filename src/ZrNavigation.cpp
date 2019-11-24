#include "ZrDebug.h"

#include <bits/stdc++.h>
#include <iterator>
#include <utility>
#include <vector>

#include "ZrComm.h"
#include "ZrFuncType.h"
#include "ZrFuncTypeDescription.h"
#include "ZrFunction.h"
#include "ZrGlobal.h"
#include "ZrNavigation.h"
#include "ZrPage.h"

// static const int H_COUNT = 16;
// static const int V_COUNT = 3;
static const int PAGE_COUNT = 2;

ZrPage m_currentPage;
// TODO
ZrFunction m_functions[ZrPage::H_COUNT * ZrPage::V_COUNT];
// TODO: capacity!
std::map<std::string, ZrFunction*> m_oscAddrToFunctionMap;

// int m_currentHPos;
// int m_currentVPos;
int m_currentPageIndex;

static const int index(int h, int v) { return h + v * ZrPage::H_COUNT; }

const int index() {
  return index(m_currentPage.m_currentHPos, m_currentPage.m_currentVPos);
}

/*

linear parameter:      value = fmin + (fmax - fmin) * float
log parameter:         value = fmin * exp(log(fmax / fmin) * float)
fader/level parameter: dblevel = 40  * idx / (steps - 1) - 30; if idx >= steps /
2 80  * idx / (steps - 1) - 50; if idx >= steps / 4 160 * idx / (steps - 1) -
70; if idx >= steps / 16 480 * idx / (steps - 1) - 90; if idx > 0 -144; if idx =
0

 */

static ZrNavigation m_instance;

ZrNavigation& ZrNavigation::instance() { return m_instance; }

void ZrNavigation::buildFunctions() {
  char temp[50];

  for (int h = 0; h < m_currentPage.m_hCount; h++) {
    for (int v = 0; v < m_currentPage.m_vCount; v++) {
      int channelNumber = h + 1;
      int ind = index(h, v);
      ZrFunction& toPopulate = m_functions[ind];
      toPopulate.m_hPos = h;
      toPopulate.m_vPos = v;
      if (h < 18) {
        sprintf(temp, "CH%02d", channelNumber);
        toPopulate.m_humanChannelName = temp;
        toPopulate.m_typeDesc =
            ZrFuncTypeDescription::posToFuncTypeDescription(h, v);
        switch (toPopulate.m_typeDesc.type()) {
          case GAIN:
            sprintf(temp, "/headamp/%02d/gain", channelNumber);
            toPopulate.m_oscAddr = temp;
            break;
          case EQ:
            sprintf(temp, "/ch/%02d/eq/on", channelNumber);
            toPopulate.m_oscAddr = temp;
            break;
          case FADER:
            sprintf(temp, "/ch/%02d/mix/fader", channelNumber);
            toPopulate.m_oscAddr = temp;
            break;
          default:
            break;
        }
      }
      if (toPopulate.m_oscAddr != UNKNOWN_OSC_ADDR) {
        // TODO?
        m_oscAddrToFunctionMap.insert({toPopulate.m_oscAddr, &toPopulate});
      }
    }
  }
}

void ZrNavigation::init() {
  TRACE();
  buildFunctions();
}

const int SIZE_OF_OSC_ADDRESS_BUFFER = 100;

void ZrNavigation::updateCachedValue(OSCMessage& msg) {
  TRACE();
  char buffer[SIZE_OF_OSC_ADDRESS_BUFFER];
  msg.getAddress(buffer);

  auto it = m_oscAddrToFunctionMap.find(buffer);
  if (it == m_oscAddrToFunctionMap.end()) {
    std::string oscAddr = buffer;
    if (oscAddr.find(M_STATUS, 0) != std::string::npos) {
      ZrComm::instance().mStatusReceived(msg);
      return;
    }

    if (oscAddr.find(M_XREMOTE, 0) != std::string::npos) {
      return;
    }

    Serial.print("Warning: no cache value for: ");
    Serial.println(buffer);
    return;
  }
  ZrFunction* func = m_oscAddrToFunctionMap[buffer];

  // ZrFunction& func = m_functions[index];
  func->updateCachedValue(msg);
}

ZrFunction& ZrNavigation::currentFunction() const {
  return m_functions[index()];
}

// ch/../config/name

/**
 * @return the ZrFunction to go to when the current function is @a other and
 * the "right" button is pressed.
 */
void ZrNavigation::goRight() { m_currentPage.goRight(); }

void ZrNavigation::goLeft() { m_currentPage.goLeft(); }

void ZrNavigation::goDown() { m_currentPage.goDown(); }

void ZrNavigation::goUp() { m_currentPage.goUp(); }

void ZrNavigation::clickPlus() { currentFunction().clickPlus(); }

void ZrNavigation::clickMinus() { currentFunction().clickMinus(); }

const int ZrNavigation::getCurrentPageIndex() const {
  return m_currentPageIndex;
}

void ZrNavigation::clickPage() {
  TRACE();
  m_currentPageIndex++;
  if (m_currentPageIndex == PAGE_COUNT) {
    m_currentPageIndex = 0;
  }
}
