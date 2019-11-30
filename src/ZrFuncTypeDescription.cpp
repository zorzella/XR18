#include "ZrDebug.h"

#include <string>

#include "ZrFuncTypeDescription.h"
#include "func/ZrFader.h"
#include "func/ZrGain.h"

ZrFuncTypeDescription::ZrFuncTypeDescription(ZrFuncType type,
                                             std::string humanName,
                                             double humanNotch, PlusMinusBehavior plusMinusBehavior)
    : m_type{type},
      m_humanName{humanName},
      m_humanNotch{humanNotch},
      // m_isOnOff{isOnOff},
      m_plusMinusBehavior {plusMinusBehavior} {}

const ZrFuncType ZrFuncTypeDescription::type() const { return m_type; }

const std::string ZrFuncTypeDescription::humanName() const {
  return m_humanName;
}

const double ZrFuncTypeDescription::humanNotch() const { return m_humanNotch; }

const PlusMinusBehavior ZrFuncTypeDescription::getPlusMinusBehavior() const {
  return m_plusMinusBehavior;
}


const double ZrFuncTypeDescription::humanToOscValue(double human) const {
  switch (m_type) {
    case GAIN:
      return ZrGain::humanToOscValue(human);
    case FADER:
      return ZrFader::humanToOscValue(human);
    default:
      TRACE();
      return 0;
  }
}

const double ZrFuncTypeDescription::oscValueToHuman(double oscValue) const {
  switch (m_type) {
    case GAIN:
      return ZrGain::oscValueToHuman(oscValue);
    case FADER:
      return ZrFader::oscValueToHuman(oscValue);
    default:
      TRACE();
      return 0;
  }
}

const double ZrFuncTypeDescription::oscValueToRoundedHuman(
    double oscValue) const {
  switch (m_type) {
    case GAIN:
      return ZrGain::oscValueToRoundedHuman(oscValue);
    case FADER:
      return ZrFader::oscValueToRoundedHuman(oscValue);
    default:
      TRACE();
      return 0;
  }
}
