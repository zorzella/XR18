#include <ArduinoTrace.h>

#include <OSCMessage.h>
#include <string>

#include "XRComm.h"
#include "XRFunction.h"
#include "ZOSCValue.h"

XRFunction::XRFunction()
    : m_hPos{-1}, m_vPos{-1}, m_name{UNKNOWN}, m_oscAddr{UNKNOWN}, m_notch{0} {}

XRFunction::XRFunction(const int hPos, const int vPos, const std::string name,
                       const std::string oscAddr, const float notch)
    : m_hPos(hPos),
      m_vPos(vPos),
      m_name(name),
      m_oscAddr(oscAddr),
      m_notch(notch) {}

void plus(OSCMessage& outParam, OSCMessage& source) { outParam = source; }

const int XRFunction::hPos() const { return m_hPos; }
const int XRFunction::vPos() const { return m_vPos; }

const std::string XRFunction::name() const { return m_name; }

const std::string XRFunction::oscAddr() const { return m_oscAddr; }

const float XRFunction::notch() const { return m_notch; }

const int CACHE_TOLERANCE = 500;

void XRFunction::clickChange(const float notch) {
  bool sentRefreshRequest = false;
  long timeoutAt = millis() + 2000;
  while (millis() < timeoutAt) {
    if (m_lastUpdated + CACHE_TOLERANCE > millis()) {
      // ok, we seem to be sufficiently up-to-date
      send2(m_oscAddr, m_cachedValue.plus(notch));
      // invalidate the cache and initiate a message that will refresh it
      m_lastUpdated = -1;
      send1(m_oscAddr);
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

void XRFunction::clickPlus() {
  TRACE();
  clickChange(m_notch);
}

void XRFunction::clickMinus() {
  TRACE();
  clickChange(-m_notch);
}

void XRFunction::updateCachedValue(OSCMessage& msg) {
  TRACE();
  if (msg.size() == 0) {
    Serial.println("Too few data points!");
    return;
  }
  if (msg.size() > 1) {
    Serial.println("Too many data points! Ignoring.");
  }
  m_lastUpdated = millis();
  m_cachedValue = ZOSCValue(msg, 0);
}
