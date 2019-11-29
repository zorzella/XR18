#include "ZrDebug.h"

#include <OSCMessage.h>
#include <iostream>
#include <sstream>
#include <string>

#include "ZoscValue.h"
#include "ZrFuncTypeDescription.h"

ZoscValue::ZoscValue()
    : m_isPresent{false},
      m_type{ZOSC_UNKNOWN},
      m_asStrOsc{""},
      m_asStrHuman{""} {}

void ZoscValue::setMessage(const ZrFuncTypeDescription& typeDesc,
                           OSCMessage& msg, int index) {
  m_isPresent = true;

  OSCData* oscData = msg.getOSCData(index);

  std::ostringstream strsOsc;
  std::ostringstream strsHuman;
  if (oscData->type == 'i') {
    m_type = ZOSC_I;
    m_data.i = oscData->getInt();
    strsOsc << m_data.i;
    if (typeDesc.isOnOff()) {
      strsHuman << (m_data.i == 0 ? "OFF" : "ON");
    } else {
      strsHuman << typeDesc.oscValueToRoundedHuman(m_data.i);
    }
  } else if (oscData->type == 'f') {
    m_type = ZOSC_F;
    m_data.f = oscData->getFloat();
    strsOsc << m_data.f;
    strsHuman << typeDesc.oscValueToRoundedHuman(m_data.f);
  } else if (oscData->type == 'd') {
    m_type = ZOSC_D;
    m_data.d = oscData->getDouble();
    strsOsc << m_data.d;
    strsHuman << typeDesc.oscValueToRoundedHuman(m_data.d);
  } else if (oscData->type == 's') {
    m_type = ZOSC_S;
    m_data.s = new char[oscData->bytes];
    oscData->getString(m_data.s);
    strsOsc << m_data.s;
    strsHuman << m_data.s;
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

static const bool QUANTIZE = true;

// If QUANTIZE is false, simply return {value}
// If QUANTIZE is true, quantize (round) {value} on {notch}
// increments. I.e.
static double maybeQuantize(const double value, const double notch) {
  if (!QUANTIZE) {
    return value;
  }
  double step = 1.0 / notch;
  int temp = (int)round(value * step);
  double result = temp / step;

  // Serial.println("Quantization:");
  // Serial.println(value);
  // Serial.println(notch);
  // Serial.println(step);
  // Serial.println(temp);
  // Serial.println(result);

  return result;
}

void ZoscValue::plus(const ZrFuncTypeDescription& typeDesc,
                     const double humanNotch) {
  switch (m_type) {
    case ZOSC_I: {
      float oldHuman = typeDesc.oscValueToHuman(m_data.i);
      float newHuman = maybeQuantize(oldHuman + humanNotch, humanNotch);
      float newOscData = typeDesc.humanToOscValue(newHuman);
      m_data.i = newOscData;
    } break;
    case ZOSC_F: {
      float oldHuman = typeDesc.oscValueToHuman(m_data.f);
      float newHuman = maybeQuantize(oldHuman + humanNotch, humanNotch);
      float newOscData = typeDesc.humanToOscValue(newHuman);
      m_data.f = newOscData;
    } break;
    case ZOSC_D: {
      double oldHuman = typeDesc.oscValueToHuman(m_data.d);
      double newHuman = maybeQuantize(oldHuman + humanNotch, humanNotch);
      double newOscData = typeDesc.humanToOscValue(newHuman);
      m_data.d = newOscData;
    } break;
    default:
      printNotYetImplemented();
      break;
  }
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
