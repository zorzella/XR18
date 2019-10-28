#ifndef ZR_FUNCTION_h
#define ZR_FUNCTION_h

#include <OSCMessage.h>
#include <string>

#include "ZoscValue.h"

static const char UNKNOWN[] = "Unknown";

class ZrFunction {
 public:
  ZrFunction();

  ZrFunction(const ZrFunction& other) = delete;

  ZrFunction(const int hPos, const int vPos, const std::string name,
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
  friend class ZrNavigation;

  int m_hPos;
  int m_vPos;

  std::string m_name;
  std::string m_oscAddr;
  float m_notch;

  // The timestamp when m_cachedValue was last updated (by updateCachedValue)
  unsigned long m_lastUpdated;

  // The last known value of this function.
  ZoscValue m_cachedValue;

  void clickChange(const float notch);
};

#endif