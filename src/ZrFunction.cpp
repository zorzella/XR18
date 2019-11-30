#include "ZrDebug.h"

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
      m_typeDesc{TYPE_UNKNOWN, "Unknown", 0.0, NONE},
      m_readValueOscAddr{UNKNOWN_OSC_ADDR},
      m_actionOscAddr{UNKNOWN_OSC_ADDR},
      m_humanChannelName{"??"},
      m_humanCustomName{""},
      m_lastUpdated{0},
      m_lastSentUpdateRequest{0},
      m_cachedValue{} {}

void plus(OSCMessage& outParam, OSCMessage& source) { outParam = source; }

const int ZrFunction::hPos() const { return m_hPos; }
const int ZrFunction::vPos() const { return m_vPos; }

const std::string ZrFunction::humanChannelName() const {
  return m_humanChannelName;
}

const std::string ZrFunction::humanFunctionName() const {
  return m_typeDesc.humanName();
}

const std::string ZrFunction::readValueOscAddr() const {
  return m_readValueOscAddr;
}

const float ZrFunction::notch() const { return m_typeDesc.humanNotch(); }

const ZoscValue& ZrFunction::cachedValue() const { return m_cachedValue; }

static const int CACHE_TOLERANCE = 2000;

const bool ZrFunction::cacheIsStale() const {
  return m_lastUpdated + CACHE_TOLERANCE < millis();
};

const bool ZrFunction::lastCacheUpdateRequestIsOld() const {
  return m_lastSentUpdateRequest + CACHE_TOLERANCE < millis();
};

bool ZrFunction::triggerCacheUpdate() { return send1(m_readValueOscAddr); }

void ZrFunction::triggerCacheUpdateIfNeeded() {
  if (!ZrComm::instance().isConnectedToXr()) {
    return;
  }
  if (cacheIsStale() && lastCacheUpdateRequestIsOld()) {
    if (triggerCacheUpdate()) {
      m_lastSentUpdateRequest = millis();
    }
  }
}

void ZrFunction::clickChange(const double humanNotch) {
  TRACE();
  bool sentRefreshRequest = false;
  long timeoutAt = millis() + 2000;
  while (millis() < timeoutAt) {
    if (!cacheIsStale()) {
      Serial.println("Cached value is up-to-date.");
      // ok, we seem to be sufficiently up-to-date
      ZoscValue copy = m_cachedValue;
      copy.plus(m_typeDesc, humanNotch);
      send2(m_actionOscAddr, copy);
      // invalidate the cache and initiate a message that will refresh it
      m_lastUpdated = 0;
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

void ZrFunction::clickPlus() {
  TRACE();
  switch (m_typeDesc.getPlusMinusBehavior()) {
    case ON_OFF:
      send2(m_actionOscAddr, FEATURE_ON);
      triggerCacheUpdate();
      break;
    case INC_DEC:
      clickChange(m_typeDesc.humanNotch());
      break;
    case LOAD_NONE:
      send2(m_actionOscAddr, m_vPos + 1);
      break;
    case NONE:
      break;
    default:
      Serial.print("Unimplemented plus minus: ");
      Serial.println(m_typeDesc.getPlusMinusBehavior());
      break;
  }
}

void ZrFunction::clickMinus() {
  TRACE();
  switch (m_typeDesc.getPlusMinusBehavior()) {
    case ON_OFF:
      send2(m_actionOscAddr, FEATURE_OFF);
      triggerCacheUpdate();
    case INC_DEC:
      clickChange(-(m_typeDesc.humanNotch()));
    case NONE:
      break;
    case LOAD_NONE:
      break;
    default:
      Serial.print("Unimplemented plus minus: ");
      Serial.println(m_typeDesc.getPlusMinusBehavior());
      break;
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

  Serial.print(", m_readValueOscAddr: ");
  Serial.print(m_readValueOscAddr.c_str());
  Serial.print(", m_actionOscAddr: ");
  Serial.print(m_actionOscAddr.c_str());
  Serial.print(", m_lastUpdated: ");
  Serial.println(m_lastUpdated);
}
