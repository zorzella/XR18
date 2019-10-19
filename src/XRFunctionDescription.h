#ifndef XR_FUNCTION_DESCRIPTION_h
#define XR_FUNCTION_DESCRIPTION_h

#include <OSCMessage.h>

#include <string>

class XRFunctionDescription {
 private:
  const std::string m_oscAddr;
  const std::string m_name;

 public:
  XRFunctionDescription(const std::string oscAddr, const std::string name);
  const std::string oscAddr();
  const std::string name();
  void plus(OSCMessage &outParam, OSCMessage &source);
};

#endif