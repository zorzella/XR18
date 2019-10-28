#include <ArduinoTrace.h>

#include <OSCMessage.h>
#include <string>

#include "ZoscValue.h"
#include "ZrComm.h"
#include "ZrFuncType.h"
#include "ZrFuncTypeDescription.h"
#include "ZrFunction.h"

ZrFunction::ZrFunction() : m_hPos{-1}, m_vPos{-1}, m_oscAddr{UNKNOWN} {}

ZrFunction::ZrFunction(const int hPos, const int vPos,
                       const std::string oscAddr)
    : m_hPos(hPos), m_vPos(vPos), m_oscAddr(oscAddr) {}

void plus(OSCMessage& outParam, OSCMessage& source) { outParam = source; }

const int ZrFunction::hPos() const { return m_hPos; }
const int ZrFunction::vPos() const { return m_vPos; }

const std::string ZrFunction::humanName() const {
  return m_typeDesc.humanName();
}

const std::string ZrFunction::oscAddr() const { return m_oscAddr; }

const float ZrFunction::notch() const { return m_typeDesc.humanNotch(); }

const int CACHE_TOLERANCE = 500;

void ZrFunction::clickChange(const float notch) {
  bool sentRefreshRequest = false;
  long timeoutAt = millis() + 2000;
  while (millis() < timeoutAt) {
    Serial.print("m_oscAddr: ");
    Serial.print(m_oscAddr.c_str());
    Serial.print(", m_lastUpdated: ");
    Serial.println(m_lastUpdated);
    if (m_lastUpdated + CACHE_TOLERANCE > millis()) {
      Serial.println("Cached value is up-to-date.");
      // ok, we seem to be sufficiently up-to-date
      send2(m_oscAddr, m_cachedValue.plus(notch));
      // invalidate the cache and initiate a message that will refresh it
      m_lastUpdated = -1;
      send1(m_oscAddr);
      return;
    } else if (!sentRefreshRequest) {
      if (send1(m_oscAddr)) {
        sentRefreshRequest = true;
        Serial.print("Waiting for current value");
      } else {
        Serial.println("Failed");
        return;
      }
    }
    OSCMessage msg;
    receiveOscIfAny(msg);
    if (msg.size() == 0) {
      Serial.print(".");
      delay(20);
    } else {
      Serial.println(" Message received.");
    }
  }
  Serial.println("\n Timed out in clickChange.");
}

void ZrFunction::clickPlus() {
  TRACE();
  clickChange(m_typeDesc.humanNotch());
}

void ZrFunction::clickMinus() {
  TRACE();
  clickChange(-(m_typeDesc.humanNotch()));
}

void ZrFunction::updateCachedValue(OSCMessage& msg) {
  TRACE();
  if (msg.size() == 0) {
    Serial.println("Too few data points!");
    return;
  }
  if (msg.size() > 1) {
    Serial.println("Too many data points! Ignoring.");
  }
  m_lastUpdated = millis();
  m_cachedValue = ZoscValue(msg, 0);
  printRec(msg);
  Serial.print("New cached value: ");
  Serial.print(m_cachedValue.asStr().c_str());

  Serial.print(", m_oscAddr: ");
  Serial.print(m_oscAddr.c_str());
  Serial.print(", m_lastUpdated: ");
  Serial.println(m_lastUpdated);
}
