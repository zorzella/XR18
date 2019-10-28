#include <bits/stdc++.h>
#include <iterator>
#include <utility>
#include <vector>

#include <ArduinoTrace.h>

#include "ZrFuncType.h"
#include "ZrFuncTypeDescription.h"
#include "ZrFunction.h"
#include "ZrNavigation.h"

static const int H_COUNT = 16;
static const int V_COUNT = 2;

ZrFunction m_functions[H_COUNT * V_COUNT];
// TODO: capacity!
std::map<std::string, int> m_oscAddrToFunctionsArrayIndexMap;

int m_currentHPos;
int m_currentVPos;

static const int index(int h, int v) { return h + v * H_COUNT; }

const int index() { return index(m_currentHPos, m_currentVPos); }

/*

linear parameter:      value = fmin + (fmax - fmin) * float
log parameter:         value = fmin * exp(log(fmax / fmin) * float)
fader/level parameter: dblevel = 40  * idx / (steps - 1) - 30; if idx >= steps /
2 80  * idx / (steps - 1) - 50; if idx >= steps / 4 160 * idx / (steps - 1) -
70; if idx >= steps / 16 480 * idx / (steps - 1) - 90; if idx > 0 -144; if idx =
0

 */

void ZrNavigation::buildFunctions() {
  char temp[50];

  for (int h = 0; h < H_COUNT; h++) {
    for (int v = 0; v < V_COUNT; v++) {
      int channelNumber = h + 1;
      int ind = index(h, v);
      ZrFunction& toPopulate = m_functions[ind];
      toPopulate.m_hPos = h;
      toPopulate.m_vPos = v;
      if (h < 18) {
        toPopulate.m_typeDesc =
            ZrFuncTypeDescription::posToFuncTypeDescription(h, v);
        switch (toPopulate.m_typeDesc.type()) {
          case GAIN:
            // toPopulate.m_name = "Gain";
            sprintf(temp, "/headamp/%02d/gain", channelNumber);
            toPopulate.m_oscAddr = temp;
            // toPopulate.m_notch = 0.5;
            break;
          case FADER:
            // toPopulate.m_name = "Fader";
            sprintf(temp, "/ch/%02d/mix/fader", channelNumber);
            toPopulate.m_oscAddr = temp;
            // toPopulate.m_notch = 0.5;
            break;
          default:
            break;
        }
      }
      if (toPopulate.m_oscAddr != UNKNOWN) {
        // TODO?
        m_oscAddrToFunctionsArrayIndexMap.insert({toPopulate.m_oscAddr, ind});
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

  auto it = m_oscAddrToFunctionsArrayIndexMap.find(buffer);
  if (it == m_oscAddrToFunctionsArrayIndexMap.end()) {
    std::string oscAddr = buffer;
    if (oscAddr.find("/status", 0) != 0 || oscAddr.find("/xremote", 0) != 0) {
      return;
    }

    Serial.print("Warning: no cache value for: ");
    Serial.println(buffer);
    return;
  }
  int index = m_oscAddrToFunctionsArrayIndexMap[buffer];

  ZrFunction& func = m_functions[index];
  func.updateCachedValue(msg);
}

ZrFunction& ZrNavigation::currentFunction() const {
  return m_functions[index()];
}

// ch/../config/name

/**
 * @return the ZrFunction to go to when the current function is @a other and
 * the "right" button is pressed.
 */
void ZrNavigation::goRight() {
  TRACE();
  m_currentHPos++;
  if (m_currentHPos == H_COUNT) {
    m_currentHPos = 0;
  }
}

void ZrNavigation::goLeft() {
  TRACE();
  m_currentHPos--;
  if (m_currentHPos < 0) {
    m_currentHPos = H_COUNT - 1;
  }
}

void ZrNavigation::goDown() {
  TRACE();
  m_currentVPos++;
  if (m_currentVPos == V_COUNT) {
    m_currentVPos = 0;
  }
}

void ZrNavigation::goUp() {
  TRACE();
  m_currentVPos--;
  if (m_currentVPos < 0) {
    m_currentVPos = V_COUNT - 1;
  }
}

void ZrNavigation::clickPlus() { currentFunction().clickPlus(); }

void ZrNavigation::clickMinus() { currentFunction().clickMinus(); }
