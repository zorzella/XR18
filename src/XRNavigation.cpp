#include <bits/stdc++.h>
#include <iterator>
#include <utility>
#include <vector>

#include <ArduinoTrace.h>

#include "XRFunction.h"
#include "XRNavigation.h"

static const int H_COUNT = 16;
static const int V_COUNT = 2;

XRFunction m_functions[H_COUNT * V_COUNT];
// TODO: capacity!
std::map<std::string, int> oscAddrToFunctionsArrayIndexMap;

int m_currentHPos;
int m_currentVPos;

static const int index(int h, int v) { return h + v * H_COUNT; }

const int index() { return index(m_currentHPos, m_currentVPos); }

enum XRFunc {
  GAIN,
  FADER,
};

void XRNavigation::buildFunctions() {
  char temp[50];

  for (int h = 0; h < H_COUNT; h++) {
    for (int v = 0; v < V_COUNT; v++) {
      int ind = index(h, v);
      XRFunction& toPopulate = m_functions[ind];
      toPopulate.m_hPos = h;
      toPopulate.m_vPos = v;
      if (h < 18) {
        switch (v) {
          case GAIN:
            toPopulate.m_name = "Gain";
            sprintf(temp, "/headamp/%02d/gain", h);
            toPopulate.m_oscAddr = temp;
            toPopulate.m_notch = 0.5;
            break;
          case FADER:
            toPopulate.m_name = "Fader";
            sprintf(temp, "/ch/%02d/mix/fader", h);
            toPopulate.m_oscAddr = temp;
            toPopulate.m_notch = 0.5;
            break;
        }
      }
      if (toPopulate.m_oscAddr != UNKNOWN) {
        // TODO?
        oscAddrToFunctionsArrayIndexMap.insert({toPopulate.m_oscAddr, ind});
      }
    }
  }
}

void XRNavigation::init() {
  TRACE();
  buildFunctions();
}

XRFunction& XRNavigation::currentFunction() const {
  return m_functions[index()];
}

// ch/../config/name

/**
 * @return the XRFunction to go to when the current function is @a other and
 * the "right" button is pressed.
 */
void XRNavigation::goRight() {
  TRACE();
  m_currentHPos++;
  if (m_currentHPos == H_COUNT) {
    m_currentHPos = 0;
  }
}

void XRNavigation::goLeft() {
  TRACE();
  m_currentHPos--;
  if (m_currentHPos < 0) {
    m_currentHPos = H_COUNT - 1;
  }
}

void XRNavigation::goDown() {
  TRACE();
  m_currentVPos++;
  if (m_currentVPos == V_COUNT) {
    m_currentVPos = 0;
  }
}

void XRNavigation::goUp() {
  TRACE();
  m_currentVPos--;
  if (m_currentVPos < 0) {
    m_currentVPos = V_COUNT - 1;
  }
}

void XRNavigation::clickPlus() {
  currentFunction().clickPlus();
}

void XRNavigation::clickMinus() {
    currentFunction().clickMinus();
}
