#pragma once

#include "ZrDebug.h"

#include <string>

#include "ZrFuncType.h"

enum PlusMinusBehavior {
  NONE,
  INC_DEC,
  ON_OFF,
  LOAD_NONE,
};

class ZrFuncTypeDescription {
 public:
  ZrFuncTypeDescription() = delete;
  ZrFuncTypeDescription(ZrFuncType type, std::string humanName,
                        double humanNotch, PlusMinusBehavior plusMinusBehavior);

  const ZrFuncType type() const;
  const std::string humanName() const;
  const double humanNotch() const;
  const PlusMinusBehavior getPlusMinusBehavior() const;

  const double humanToOscValue(double human) const;
  const double oscValueToHuman(double oscValue) const;
  const double oscValueToRoundedHuman(double oscValue) const;

  static const ZrFuncTypeDescription fromType(const ZrFuncType type) {
    switch (type) {
      case GAIN:
        return ZrFuncTypeDescription(type, "Gain", 0.5, INC_DEC);
      case EQ:
        return ZrFuncTypeDescription(type, "EQ", 0.0, ON_OFF);
      case FADER:
        return ZrFuncTypeDescription(type, "Fader", 1.0, INC_DEC);
      case SNAPSHOT:
        return ZrFuncTypeDescription(type, "Name", 0.0, LOAD_NONE);
      default:
        TRACE();
        return ZrFuncTypeDescription(type, "Unknown", 0.0, NONE);
    }
  }

 private:
  // TODO: make all these const?
  ZrFuncType m_type;
  std::string m_humanName;
  double m_humanNotch;
  PlusMinusBehavior m_plusMinusBehavior;
};
