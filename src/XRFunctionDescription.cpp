#include <OSCMessage.h>  // For OSC support
#include <string>

#include "XRFunctionDescription.h"

XRFunctionDescription::XRFunctionDescription(const std::string oscAddr,
                                             const std::string name)
    : m_oscAddr(oscAddr), m_name(name) {}

const std::string XRFunctionDescription::name() { return m_name; }

const std::string XRFunctionDescription::oscAddr() { return m_oscAddr; }

void XRFunctionDescription::plus(OSCMessage &outParam, OSCMessage &source) {
  outParam = source;
}
