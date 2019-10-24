#ifndef XR_FUNCTION_h
#define XR_FUNCTION_h

#include <string>

#include <OSCMessage.h>

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
  void plus();

 private:
  friend class XRNavigation;

  int m_hPos;
  int m_vPos;

  std::string m_name;
  std::string m_oscAddr;
  float m_notch;

  unsigned long lastUpdated;
  OSCData *oscData;
};

#endif