#pragma once

#include <OSCMessage.h>
#include <inttypes.h>

#include "ZrFuncTypeDescription.h"

enum ZoscType {
  ZOSC_UNKNOWN,
  ZOSC_I,
  ZOSC_F,
  ZOSC_D,
  // ZOSC_L,
};

class ZoscValue {
 public:
  ZoscValue();
  //   ZOSCValue(const ZOSCValue& other);
  ZoscValue(const ZrFuncTypeDescription& typeDesc, OSCMessage& osc, int index);
  //   ZOSCValue& operator=(const ZOSCValue& source);

  const bool isPresent() const;

  const ZoscValue plus(const ZrFuncTypeDescription& typeDesc, const float notch) const;

  void addItselfTo(OSCMessage& msg) const;

  const std::string& asStrOsc() const;
  const std::string& asStrHuman() const;

 private:
  bool m_isPresent;

  ZoscType m_type;

  std::string m_asStrOsc;
  std::string m_asStrHuman;

  union {
    // char * s; //string
    int32_t i;  // int
    float f;    // float
    double d;   // double
    // uint64_t l;  // long
    // uint8_t * b; //blob
    // osctime_t time;
  } m_data;

  void printNotYetImplemented() const;
};