#pragma once

#include <ArduinoTrace.h>
#include <string>

enum ZrFuncType {
  TYPE_UNKNOWN,
  GAIN,   // [0.0,1.0] lin(145), -12..60 dB
  EQ,
  FADER,  // [0.0,1.0] fader(1024), -oo..10 dB
};

