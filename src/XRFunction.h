#ifndef XR_FUNCTION_h
#define XR_FUNCTION_h

#include <OSCMessage.h>
#include <string>

#include "ZOSCValue.h"

static const char UNKNOWN[] = "Unknown";

class XRFunction {
 public:
  XRFunction();

  XRFunction(const int hPos, const int vPos, const std::string name,
             const std::string oscAddr, const float notch);

  const std::string name() const;
  const std::string oscAddr() const;
  const float notch() const;
  const int hPos() const;
  const int vPos() const;
  void clickPlus();
  void clickMinus();
  void updateCachedValue(OSCMessage& msg);

 private:
  friend class XRNavigation;

  int m_hPos;
  int m_vPos;

  std::string m_name;
  std::string m_oscAddr;
  float m_notch;

  // The timestamp when m_cachedValue was last updated (by updateCachedValue)
  unsigned long m_lastUpdated;

  // The last known value of this function.
  ZOSCValue m_cachedValue;

  void clickChange(const float notch);
};

#endif