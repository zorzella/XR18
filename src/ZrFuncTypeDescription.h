#pragma once

#include <string>

#include "ZrFuncType.h"

class ZrFuncTypeDescription {
 public:
  ZrFuncTypeDescription();
  ZrFuncTypeDescription(ZrFuncType type, std::string humanName,
                        float humanNotch, boolean isOnOff);

  const ZrFuncType type() const;
  const std::string humanName() const;
  const float humanNotch() const;
  const bool isOnOff() const;

  const float humanToOscValue(float human) const;
  const float oscValueToHuman(float oscValue) const;

  static const ZrFuncTypeDescription fromType(const ZrFuncType type) {
    switch (type) {
      case GAIN:
        return ZrFuncTypeDescription(type, "Gain", 0.5, false);
      case EQ:
        return ZrFuncTypeDescription(type, "EQ", 0, true);
      case FADER:
        return ZrFuncTypeDescription(type, "Fader", 0.5, false);
      default:
        TRACE();
        return ZrFuncTypeDescription(type, "Unknown", 0, false);
    }
  }

  static const ZrFuncType fromType(int h, int v) {
    if (v == 0) {
      return GAIN;
    }
    if (v == 1) {
      return EQ;
    }
    if (v == 1) {
      return FADER;
    }
    return TYPE_UNKNOWN;
  }

  static const ZrFuncTypeDescription posToFuncTypeDescription(int h, int v) {
    return fromType(fromType(h, v));
  }

 private:
  ZrFuncType m_type;
  std::string m_humanName;
  float m_humanNotch;
  bool m_isOnOff;
};
