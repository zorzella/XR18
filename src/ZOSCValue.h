#pragma once

#include "ZrDebug.h"

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

  void setMessage(const ZrFuncTypeDescription& typeDesc, OSCMessage& msg,
                  int index);

  const bool isPresent() const;

  void plus(const ZrFuncTypeDescription& typeDesc, const double humanNotch);

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