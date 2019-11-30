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
#include "page/ZrChannelsPagePopulator.h"
#include "page/ZrMasterPagePopulator.h"
#include "page/ZrSnapshotPagePopulator.h"

static const int PAGE_COUNT = 3;

ZrPage m_channelsPage{ZrChannelsPagePopulator::V_COUNT,
                      ZrChannelsPagePopulator::H_COUNT};

ZrPage m_masterPage{ZrMasterPagePopulator::V_COUNT,
                    ZrMasterPagePopulator::H_COUNT};

ZrPage m_snapshotPage{ZrSnapshotPagePopulator::V_COUNT,
                    ZrSnapshotPagePopulator::H_COUNT};

ZrPage m_pages[3]{m_channelsPage, m_masterPage, m_snapshotPage};

// TODO: capacity!
std::map<std::string, ZrFunction*> m_oscAddrToFunctionMap;

int m_currentPageIndex;

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

ZrPage& ZrNavigation::currentPage() const {
  return m_pages[m_currentPageIndex];
}

void ZrNavigation::buildFunctions() {
  ZrChannelsPagePopulator::populate(m_pages[0]);
  ZrMasterPagePopulator::populate(m_pages[1]);
  ZrSnapshotPagePopulator::populate(m_pages[2]);

  for (ZrPage& zrPage : m_pages) {
    for (int i = 0; i < zrPage.indexCount(); i++) {
      ZrFunction& toPopulate = zrPage.m_functions[i];
      if (toPopulate.readValueOscAddr() != UNKNOWN_OSC_ADDR) {
        m_oscAddrToFunctionMap.insert({toPopulate.readValueOscAddr(), &toPopulate});
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

  func->updateCachedValue(msg);
}

ZrFunction& ZrNavigation::currentFunction() const {
  return currentPage().m_functions[currentPage().index()];
}

// ch/../config/name

/**
 * Changes the "current" function as per clicking the "right" button.
 */
void ZrNavigation::goRight() { currentPage().goRight(); }

void ZrNavigation::goLeft() { currentPage().goLeft(); }

void ZrNavigation::goDown() { currentPage().goDown(); }

void ZrNavigation::goUp() { currentPage().goUp(); }

void ZrNavigation::clickPlus() {
  if (!ZrComm::instance().isConnectedToXr()) {
    return;
  }
  currentFunction().clickPlus();
}

void ZrNavigation::clickMinus() {
  if (!ZrComm::instance().isConnectedToXr()) {
    return;
  }
  currentFunction().clickMinus();
}

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

void ZrNavigation::printDebug() {}