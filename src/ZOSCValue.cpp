#include <OSCMessage.h>
#include <iostream>
#include <sstream>
#include <string>

#include "ZoscValue.h"

ZoscValue::ZoscValue()
    : m_isPresent{false}, m_type{ZOSC_UNKNOWN}, m_asStr{""} {}

// ZOSCValue::ZOSCValue(const ZOSCValue& other) : m_isPresent { true }, m_data {
// other.m_data} {};

// ZOSCValue& ZOSCValue::operator=(const ZOSCValue& source) {
//     if (this == &source) {
//         return this;
//     }
//     // this.m_isPresent =
// }

ZoscValue::ZoscValue(OSCMessage& msg, int index) : m_isPresent{true} {
  std::ostringstream strs;

  if (msg.isInt(index)) {
    m_type = ZOSC_I;
    m_data.i = msg.getInt(index);
    strs << m_data.i;
  } else if (msg.isFloat(index)) {
    m_type = ZOSC_F;
    m_data.f = msg.getFloat(index);
    strs << m_data.f;
  } else if (msg.isDouble(index)) {
    m_type = ZOSC_D;
    m_data.d = msg.getDouble(index);
    strs << m_data.d;
  } else {
    m_type = ZOSC_UNKNOWN;
    Serial.print("OSC type not yet implemented: ");
    Serial.println(msg.getType(index));
  }
  m_asStr = strs.str();
  strs.clear();
}

const ZoscValue ZoscValue::plus(const float notch) const {
  ZoscValue result{*this};
  // if ()
  switch (m_type) {
    case ZOSC_I:
      result.m_data.i += notch;
      break;
    case ZOSC_F:
      result.m_data.f += notch;
      break;
    case ZOSC_D:
      result.m_data.d += notch;
      break;
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

const std::string& ZoscValue::asStr() const { return m_asStr; }
