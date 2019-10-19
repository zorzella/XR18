#include <ArduinoTrace.h>

#include <OSCMessage.h>
#include <string>

#include "XRFunction.h"
#include "XRFunctionDescription.h"
#include "XRRowDescription.h"

XRFunction::XRFunction(const XRRowDescription& row,
                       const XRFunctionDescription& func, int hPos, int vPos)
    : m_row({row.rowName(), row.funcs()}), m_func(func), m_hPos(hPos), m_vPos(vPos) {}

const XRRowDescription XRFunction::row() const { return m_row; };
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
  // this;
  // TRACE();
  this->m_row;
  // TRACE();
  // DUMP(m_row);
  TRACE();
  this->m_row.rowName();
  TRACE();
  this->m_row.rowName().c_str();
  TRACE();
  this->m_func.name().c_str();
  TRACE();
  sprintf(buffer, "Row: %s, Func: %s", this->m_row.rowName().c_str(),
          this->m_func.name().c_str());
  TRACE();
  return buffer;
  // return "foo";
}

const int XRFunction::hPos() const { return m_hPos; }
const int XRFunction::vPos() const { return m_vPos; }
