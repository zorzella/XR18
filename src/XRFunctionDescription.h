#ifndef XR_FUNCTION_DESCRIPTION_h
#define XR_FUNCTION_DESCRIPTION_h

#include <OSCMessage.h>

#include <string>

class XRFunctionDescription {
 private:
  const std::string m_name;
  const std::string m_oscAddr;
  const float m_notch;

 public:
  XRFunctionDescription(const std::string name, const std::string oscAddr, const float notch);
  const std::string name();
  const std::string oscAddr();
  void plus(OSCMessage &outParam, OSCMessage &source);
};

#endif