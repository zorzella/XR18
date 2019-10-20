#include <ArduinoTrace.h>

#include <OSCMessage.h>
#include <string>

#include "XRFunction.h"
#include "XRFunctionDescription.h"
#include "XRRowDescription.h"

XRFunction::XRFunction(
                       const XRFunctionDescription func, int hPos, int vPos)
    : m_func(func), m_hPos(hPos), m_vPos(vPos) {}

const XRFunctionDescription XRFunction::func() const { return m_func; };

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
  this->m_func.name();
  TRACE();
  const char* oscAddr = this->m_func.oscAddr().c_str();
  TRACE();
  const char* funcName = this->m_func.name().c_str();
  TRACE();
  sprintf(buffer, "Placeholder");
  TRACE();
  sprintf(buffer, "Placeholder %i", 10);
  TRACE();
  sprintf(buffer, "Placeholder %s", "foo");
  TRACE();
  sprintf(buffer, "Placeholder %s", oscAddr);
  TRACE();
  sprintf(buffer, "Row: %s, Func: %s", oscAddr, funcName);
  TRACE();
  // sprintf(buffer, "Row: %s, Func: %s", this->m_row.rowName().c_str(),
  //         this->m_func.name().c_str());
  // TRACE();
  return buffer;
  // return "foo";
}

const int XRFunction::hPos() const { return m_hPos; }
const int XRFunction::vPos() const { return m_vPos; }
