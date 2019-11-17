#ifndef ZR_NAVIGATION_h
#define ZR_NAVIGATION_h

#include "ZrDebug.h"

#include <vector>

#include "ZrFunction.h"

class ZrNavigation {
 public:
  static ZrNavigation& instance();

  void init();

  void updateCachedValue(OSCMessage& msg);

  ZrFunction& currentFunction() const;
  void goRight();
  void goLeft();
  void goUp();
  void goDown();

  void clickPlus();
  void clickMinus();

 private:
  void buildFunctions();
};

#endif