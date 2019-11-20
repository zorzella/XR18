#pragma once

#include "ZrDebug.h"

#include <string>

#include "ZrFuncType.h"

class ZrFuncTypeDescription {
 public:
  ZrFuncTypeDescription();
  ZrFuncTypeDescription(ZrFuncType type, std::string humanName,
                        double humanNotch, boolean isOnOff);

  const ZrFuncType type() const;
  const std::string humanName() const;
  const double humanNotch() const;
  const bool isOnOff() const;

  const double humanToOscValue(double human) const;
  const double oscValueToHuman(double oscValue) const;
  const double oscValueToRoundedHuman(double oscValue) const;

  static const ZrFuncTypeDescription fromType(const ZrFuncType type) {
    switch (type) {
      case GAIN:
        return ZrFuncTypeDescription(type, "Gain", 0.5, false);
      case EQ:
        return ZrFuncTypeDescription(type, "EQ", 0.0, true);
      case FADER:
        return ZrFuncTypeDescription(type, "Fader", 1.0, false);
      default:
        TRACE();
        return ZrFuncTypeDescription(type, "Unknown", 0.0, false);
    }
  }

  static const ZrFuncType typeForPosition(int h, int v) {
    if (h >= 0 && h <= 17) {
      switch (v) {
        case 0:
          return GAIN;
        case 1:
          return EQ;
        case 2:
          return FADER;
      }
    }
    return TYPE_UNKNOWN;
  }

  static const ZrFuncTypeDescription posToFuncTypeDescription(int h, int v) {
    return fromType(typeForPosition(h, v));
  }

 private:
  ZrFuncType m_type;
  std::string m_humanName;
  double m_humanNotch;
  bool m_isOnOff;
};
