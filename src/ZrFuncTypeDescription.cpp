#include <string>

#include "ZrFuncTypeDescription.h"

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
