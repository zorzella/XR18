#include <ArduinoTrace.h>

#include <OSCMessage.h>
#include <string>

#include "XRComm.h"
#include "XRFunction.h"

XRFunction::XRFunction()
    : m_hPos{-1},
      m_vPos{-1},
      m_name{UNKNOWN},
      m_oscAddr{UNKNOWN},
      m_notch{0} {}

XRFunction::XRFunction(const int hPos, const int vPos, const std::string name,
                       const std::string oscAddr, const float notch)
    : m_hPos(hPos),
      m_vPos(vPos),
      m_name(name),
      m_oscAddr(oscAddr),
      m_notch(notch) {}

void plus(OSCMessage& outParam, OSCMessage& source) { outParam = source; }

const int XRFunction::hPos() const { return m_hPos; }
const int XRFunction::vPos() const { return m_vPos; }

const std::string XRFunction::name() const { return m_name; }

const std::string XRFunction::oscAddr() const { return m_oscAddr; }

const float XRFunction::notch() const { return m_notch; }

void XRFunction::clickPlus() {
  // send1()
}

void XRFunction::clickMinus() {

}
