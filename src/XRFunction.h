#ifndef XR_FUNCTION_h
#define XR_FUNCTION_h

#include <string>

#include "XRRowDescription.h"
#include "XRFunctionDescription.h"

class XRFunction {
 private:
  const XRRowDescription m_row;
  const XRFunctionDescription m_func;
  const int m_hPos;
  const int m_vPos;

 public:
  XRFunction(const XRRowDescription &m_row, const XRFunctionDescription &m_func, int hPos, int vPos);
  const XRRowDescription row() const;
  const XRFunctionDescription func() const;
  const int hPos() const;
  const int vPos() const;
};

#endif