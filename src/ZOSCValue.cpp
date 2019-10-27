#include <OSCMessage.h>

#include "ZOSCValue.h"

ZOSCValue::ZOSCValue() : m_isPresent{false}, m_type{ZOSC_UNKNOWN} {}

// ZOSCValue::ZOSCValue(const ZOSCValue& other) : m_isPresent { true }, m_data {
// other.m_data} {};

// ZOSCValue& ZOSCValue::operator=(const ZOSCValue& source) {
//     if (this == &source) {
//         return this;
//     }
//     // this.m_isPresent =
// }

ZOSCValue::ZOSCValue(OSCMessage& msg, int index) : m_isPresent{true} {
  if (msg.isInt(index)) {
    m_type = ZOSC_I;
    m_data.i = msg.getInt(index);
  } else if (msg.isFloat(index)) {
    m_type = ZOSC_F;
    m_data.f = msg.getFloat(index);
  } else {
    m_type = ZOSC_UNKNOWN;
    Serial.print("Type not yet implemented: ");
    Serial.println(msg.getType(index));
  }
}

const ZOSCValue ZOSCValue::plus(const float notch) const {
  ZOSCValue result{*this};
  // if ()
  switch (m_type) {
    case ZOSC_I:
      result.m_data.i += notch;
      break;
    default:
      printNotYetImplemented();
      break;
  }
  return result;
}

// TODO: keep a string m_asStr;
const void ZOSCValue::print() const {
  switch (m_type) {
    case ZOSC_I:
      Serial.println(m_data.i);
      break;
    default:
      Serial.print("Unknown");
      Serial.print("Type not yet implemented: ");
      Serial.println(m_type);
      break;
  }
  return;
}

void ZOSCValue::addItselfTo(OSCMessage& msg) const {
  switch (m_type) {
    case ZOSC_I:
      msg.add(m_data.i);
      break;
    default:
      printNotYetImplemented();
  }
}

void ZOSCValue::printNotYetImplemented() const {
  Serial.print("Type not yet implemented: ");
  Serial.println(m_type);
}