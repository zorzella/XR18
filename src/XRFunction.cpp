#include <OSCMessage.h>  // For OSC support
#include <string>

#include "XRFunction.h"
#include "XRFunctionDescription.h"
#include "XRRowDescription.h"

XRFunction::XRFunction(const XRRowDescription &row,
                       const XRFunctionDescription &func,
                       int hPos,
                       int vPos)
    : m_row(row), m_func(func), m_hPos(hPos), m_vPos(vPos) {}

const XRRowDescription XRFunction::row() const { return m_row; };
const XRFunctionDescription XRFunction::func() const { return m_func; };

void plus(OSCMessage& outParam, OSCMessage& source) {
  // source.getAddress(buffer);
  outParam = source;
  // if (source.ge)
  // outParam
}
