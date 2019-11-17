#include "ZrDebug.h"

#include <ArduinoTrace.h>
#include <OSCMessage.h>
#include <string>

#include "ZoscValue.h"
#include "ZrComm.h"
#include "ZrFuncType.h"
#include "ZrFuncTypeDescription.h"
#include "ZrFunction.h"

ZrFunction::ZrFunction()
    : m_hPos{-1},
      m_vPos{-1},
      m_oscAddr{UNKNOWN_OSC_ADDR},
      m_humanChannelName{"??"},
      m_humanCustomName{""} {}

void plus(OSCMessage& outParam, OSCMessage& source) { outParam = source; }

const int ZrFunction::hPos() const { return m_hPos; }
const int ZrFunction::vPos() const { return m_vPos; }

const std::string ZrFunction::humanChannelName() const {
  return m_humanChannelName;
}

const std::string ZrFunction::humanFunctionName() const {
  return m_typeDesc.humanName();
}

const std::string ZrFunction::oscAddr() const { return m_oscAddr; }

const float ZrFunction::notch() const { return m_typeDesc.humanNotch(); }

const ZoscValue& ZrFunction::cachedValue() const { return m_cachedValue; }

static const int CACHE_TOLERANCE = 2000;

const bool ZrFunction::cacheIsStale() const {
  return m_lastUpdated + CACHE_TOLERANCE < millis();
};

const bool ZrFunction::lastCacheUpdateRequestIsOld() const {
  return m_lastSentUpdateRequest + CACHE_TOLERANCE < millis();
};

bool ZrFunction::triggerCacheUpdate() { return send1(m_oscAddr); }

bool ZrFunction::triggerCacheUpdateIfNeeded() {
  if (cacheIsStale() && lastCacheUpdateRequestIsOld()) {
    m_lastSentUpdateRequest = millis();
    return send1(m_oscAddr);
  }
  return true;
}

void ZrFunction::clickChange(const float humanNotch) {
  TRACE();
  bool sentRefreshRequest = false;
  long timeoutAt = millis() + 2000;
  while (millis() < timeoutAt) {
    Serial.print("m_oscAddr: ");
    Serial.print(m_oscAddr.c_str());
    Serial.print(", m_lastUpdated: ");
    Serial.println(m_lastUpdated);
    if (!cacheIsStale()) {
      Serial.println("Cached value is up-to-date.");
      // ok, we seem to be sufficiently up-to-date
      ZoscValue copy = m_cachedValue;
      copy.plus(m_typeDesc, humanNotch);
      send2(m_oscAddr, copy);
      // invalidate the cache and initiate a message that will refresh it
      m_lastUpdated = -1;
      triggerCacheUpdate();
      return;
    } else if (!sentRefreshRequest) {
      if (triggerCacheUpdate()) {
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

const void ZrFunction::send(const std::string& msg) const {
  send2(m_oscAddr, msg);
}

void ZrFunction::clickPlus() {
  TRACE();
  if (m_typeDesc.isOnOff()) {
    send(FEATURE_ON);
  } else {
    clickChange(m_typeDesc.humanNotch());
  }
}

void ZrFunction::clickMinus() {
  TRACE();
  if (m_typeDesc.isOnOff()) {
    send(FEATURE_OFF);
  } else {
    clickChange(-(m_typeDesc.humanNotch()));
  }
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
  m_cachedValue.setMessage(m_typeDesc, msg, 0);
  printRec(msg);
  Serial.print("New cached value: ");
  Serial.print(m_cachedValue.asStrOsc().c_str());

  Serial.print(", m_oscAddr: ");
  Serial.print(m_oscAddr.c_str());
  Serial.print(", m_lastUpdated: ");
  Serial.println(m_lastUpdated);
}
