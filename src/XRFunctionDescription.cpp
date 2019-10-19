#include <OSCMessage.h>
#include <string>

#include "XRFunctionDescription.h"

XRFunctionDescription::XRFunctionDescription(const std::string name,
                                             const std::string oscAddr,
                                             const float notch)
    : m_name(name), m_oscAddr(oscAddr), m_notch(notch) {}

const std::string XRFunctionDescription::name() const { return m_name; }

const std::string XRFunctionDescription::oscAddr() const { return m_oscAddr; }

void XRFunctionDescription::plus(OSCMessage &outParam,
                                 OSCMessage &source) const {
  outParam = source;
}
