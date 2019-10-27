#pragma once

#include <OSCMessage.h>
#include <inttypes.h>

enum ZOSCType {
  ZOSC_UNKNOWN,
  ZOSC_I,
  ZOSC_F,
  ZOSC_D,
  ZOSC_L,
};

class ZOSCValue {
 public:
  ZOSCValue();
  //   ZOSCValue(const ZOSCValue& other);
  ZOSCValue(OSCMessage& osc, int index);
  //   ZOSCValue& operator=(const ZOSCValue& source);

  const bool isPresent() const;

  const ZOSCValue plus(const float notch) const;

  const void print() const;

  void addItselfTo(OSCMessage& msg) const;

 private:
  bool m_isPresent;

  ZOSCType m_type;

  union {
    // char * s; //string
    int32_t i;   // int
    float f;     // float
    double d;    // double
    uint64_t l;  // long
                 // uint8_t * b; //blob
    // osctime_t time;
  } m_data;

  void printNotYetImplemented() const;
};