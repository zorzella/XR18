#include "ZrDebug.h"

#include <string>

#include "ZrFuncTypeDescription.h"
#include "func/ZrFader.h"
#include "func/ZrGain.h"

ZrFuncTypeDescription::ZrFuncTypeDescription()
    : m_type{TYPE_UNKNOWN},
      m_humanName{"Unknown"},
      m_humanNotch{0},
      m_isOnOff{false} {}

ZrFuncTypeDescription::ZrFuncTypeDescription(ZrFuncType type,
                                             std::string humanName,
                                             float humanNotch, boolean isOnOff)
    : m_type{type},
      m_humanName{humanName},
      m_humanNotch{humanNotch},
      m_isOnOff{isOnOff} {}

const ZrFuncType ZrFuncTypeDescription::type() const { return m_type; }

const std::string ZrFuncTypeDescription::humanName() const {
  return m_humanName;
}

const float ZrFuncTypeDescription::humanNotch() const { return m_humanNotch; }

const bool ZrFuncTypeDescription::isOnOff() const {
  return m_isOnOff;
}

const float ZrFuncTypeDescription::humanToOscValue(float human) const {
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

const float ZrFuncTypeDescription::oscValueToHuman(float oscValue) const {
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
