#ifndef XR_FUNCTION_h
#define XR_FUNCTION_h

#include <string>

#include "XRFunctionDescription.h"
#include "XRRowDescription.h"

class XRFunction {
 private:
  XRFunctionDescription m_func;
  int m_hPos;
  int m_vPos;

 public:
  XRFunction(const XRFunctionDescription m_func,
             int hPos, int vPos);
  const XRFunctionDescription func() const;
  const int hPos() const;
  const int vPos() const;

  const std::string toStr() const;
};

#endif