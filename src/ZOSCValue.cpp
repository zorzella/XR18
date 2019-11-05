#include <OSCMessage.h>
#include <iostream>
#include <sstream>
#include <string>

#include "ZoscValue.h"
#include "ZrFuncTypeDescription.h"

ZoscValue::ZoscValue()
    : m_isPresent{false}, m_type{ZOSC_UNKNOWN}, m_asStrOsc{""}, m_asStrHuman{""} {}

ZoscValue::ZoscValue(const ZrFuncTypeDescription& typeDesc, OSCMessage& msg,
                     int index)
    : m_isPresent{true} {
  std::ostringstream strsOsc;
  std::ostringstream strsHuman;

  if (msg.isInt(index)) {
    m_type = ZOSC_I;
    m_data.i = msg.getInt(index);
    strsOsc << m_data.i;
    strsHuman << typeDesc.oscValueToHuman(m_data.i);
  } else if (msg.isFloat(index)) {
    m_type = ZOSC_F;
    m_data.f = msg.getFloat(index);
    strsOsc << m_data.f;
    strsHuman << typeDesc.oscValueToHuman(m_data.f);
  } else if (msg.isDouble(index)) {
    m_type = ZOSC_D;
    m_data.d = msg.getDouble(index);
    strsOsc << m_data.d;
    strsHuman << typeDesc.oscValueToHuman(m_data.d);
 } else {
    m_type = ZOSC_UNKNOWN;
    Serial.print("OSC type not yet implemented: ");
    Serial.println(msg.getType(index));
  }
  m_asStrOsc = strsOsc.str();
  strsOsc.clear();
  m_asStrHuman = strsHuman.str();
  strsHuman.clear();
}

const ZoscValue ZoscValue::plus(const ZrFuncTypeDescription& typeDesc,
                                const float humanNotch) const {
  // float human = m_typeDesc.oscValueToHuman(m_cachedValue);
  ZoscValue result{*this};
  switch (m_type) {
    case ZOSC_I: {
      float oldHuman = typeDesc.oscValueToHuman(result.m_data.i);
      float newHuman = oldHuman + humanNotch;
      float newOscData = typeDesc.humanToOscValue(newHuman);
      result.m_data.i = newOscData;
    } break;
    case ZOSC_F: {
      float oldHuman = typeDesc.oscValueToHuman(result.m_data.f);
      float newHuman = oldHuman + humanNotch;
      float newOscData = typeDesc.humanToOscValue(newHuman);
      result.m_data.f = newOscData;
    } break;
    case ZOSC_D: {
      float oldHuman = typeDesc.oscValueToHuman(result.m_data.d);
      float newHuman = oldHuman + humanNotch;
      float newOscData = typeDesc.humanToOscValue(newHuman);
      result.m_data.d = newOscData;
    } break;
    default:
      printNotYetImplemented();
      break;
  }
  return result;
}

void ZoscValue::addItselfTo(OSCMessage& msg) const {
  switch (m_type) {
    case ZOSC_I:
      msg.add(m_data.i);
      break;
    case ZOSC_F:
      msg.add(m_data.f);
      break;
    case ZOSC_D:
      msg.add(m_data.d);
      break;
    default:
      printNotYetImplemented();
  }
}

void ZoscValue::printNotYetImplemented() const {
  Serial.print("Type not yet implemented: ");
  Serial.println(m_type);
}

const std::string& ZoscValue::asStrOsc() const { return m_asStrOsc; }

const std::string& ZoscValue::asStrHuman() const { return m_asStrHuman; }
