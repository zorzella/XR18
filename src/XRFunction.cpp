#include <ArduinoTrace.h>

#include <OSCMessage.h>
#include <string>

#include "XRFunction.h"
#include "XRFunctionDescription.h"
#include "XRRowDescription.h"

XRFunction::XRFunction(const std::string name, const std::string oscAddr,
                       const float notch, int hPos, int vPos)
    : m_name(name),
      m_oscAddr(oscAddr),
      m_notch(notch),
      m_hPos(hPos),
      m_vPos(vPos) {}

void plus(OSCMessage& outParam, OSCMessage& source) {
  // source.getAddress(buffer);
  outParam = source;
  // if (source.ge)
  // outParam
}

const std::string XRFunction::toStr() const {
  char buffer[50];
  TRACE();
  Serial.println(this->m_hPos);
  TRACE();
  this->name();
  TRACE();
  const char* oscAddr = this->oscAddr().c_str();
  TRACE();
  DUMP(oscAddr);
  const char* funcName = this->name().c_str();
  TRACE();
  DUMP(funcName);
  sprintf(buffer, "Placeholder %s", "foo");
  Serial.println(buffer);
  sprintf(buffer, "Placeholder %s", "bar");
  const char* baz = "baz";
  Serial.println(buffer);
  sprintf(buffer, "Placeholder %s", baz);
  Serial.println(buffer);
  TRACE();
  sprintf(buffer, "Placeholder %s", oscAddr);
  Serial.println(buffer);
  TRACE();
  sprintf(buffer, "Row: %s, Func: %s", oscAddr, funcName);
  Serial.println(buffer);
  TRACE();
  // sprintf(buffer, "Row: %s, Func: %s", this->m_row.rowName().c_str(),
  //         this->m_func.name().c_str());
  // TRACE();
  return buffer;
  // return "foo";
}

const int XRFunction::hPos() const { return m_hPos; }
const int XRFunction::vPos() const { return m_vPos; }

const std::string XRFunction::name() const { return m_name; }

const std::string XRFunction::oscAddr() const { return m_oscAddr; }

const float XRFunction::notch() const { return m_notch; }
