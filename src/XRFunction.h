#ifndef XR_FUNCTION_h
#define XR_FUNCTION_h

#include <string>

#include "XRFunctionDescription.h"
#include "XRRowDescription.h"

class XRFunction {
 private:
  std::string m_name;
  std::string m_oscAddr;
  float m_notch;

  int m_hPos;
  int m_vPos;

 public:
  XRFunction(const std::string name, const std::string oscAddr,
             const float notch, int hPos, int vPos);

  const std::string name() const;
  const std::string oscAddr() const;
  const float notch() const;

  const int hPos() const;
  const int vPos() const;

  const std::string toStr() const;
};

#endif