#include <string>

#include "ZrFuncTypeDescription.h"
#include "ZrGain.h"

ZrFuncTypeDescription::ZrFuncTypeDescription()
    : m_type{TYPE_UNKNOWN}, m_humanName{"Unknown"}, m_humanNotch{0} {}

ZrFuncTypeDescription::ZrFuncTypeDescription(ZrFuncType type,
                                             std::string humanName,
                                             float humanNotch)
    : m_type{type}, m_humanName{humanName}, m_humanNotch{humanNotch} {}

const ZrFuncType ZrFuncTypeDescription::type() const { return m_type; }

const std::string ZrFuncTypeDescription::humanName() const {
  return m_humanName;
}

const float ZrFuncTypeDescription::humanNotch() const { return m_humanNotch; }

const float ZrFuncTypeDescription::humanToOscValue(float human) const {
  switch (m_type) {
    case GAIN:
      return ZrGain::humanToOscValue(human);
    default:
      TRACE();
      return 0;
  }
}

const float ZrFuncTypeDescription::oscValueToHuman(float oscValue) const {
  switch (m_type) {
    case GAIN:
      return ZrGain::oscValueToHuman(oscValue);
    default:
      TRACE();
      return 0;
  }
}
