#ifndef ZR_FUNCTION_h
#define ZR_FUNCTION_h

#include <OSCMessage.h>
#include <string>

#include "ZoscValue.h"
#include "ZrFuncType.h"
#include "ZrFuncTypeDescription.h"

static const char UNKNOWN_OSC_ADDR[] = "Unknown";

// Note that turning a channel mix OFF is turning Mute on
const std::string FEATURE_OFF = "OFF";
// See above
const std::string FEATURE_ON = "ON";

class ZrFunction {
 public:
  ZrFunction();

  ZrFunction(const ZrFunction& other) = delete;

  // ZrFunction(const int hPos, const int vPos, const std::string oscAddr,
  //            const std::string humanChannelName);

  const std::string humanChannelName() const;
  const std::string humanFunctionName() const;
  const std::string oscAddr() const;
  const float notch() const;
  const int hPos() const;
  const int vPos() const;
  const ZoscValue& cachedValue() const;
  void clickPlus();
  void clickMinus();
  void updateCachedValue(OSCMessage& msg);
  bool triggerCacheUpdate();
  bool triggerCacheUpdateIfNeeded();

 private:
  friend class ZrNavigation;

  int m_hPos;
  int m_vPos;

  ZrFuncTypeDescription m_typeDesc;
  std::string m_oscAddr;
  std::string m_humanChannelName;
  std::string m_humanCustomName;

  // The timestamp when m_cachedValue was last updated (by updateCachedValue)
  unsigned long m_lastUpdated;

  // The last known value of this function.
  ZoscValue m_cachedValue;

  void clickChange(const float notch);
  const void send(const std::string& msg) const;
  const bool cacheIsStale() const;
};

#endif