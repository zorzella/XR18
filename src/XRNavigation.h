#ifndef XR_NAVIGATION_h
#define XR_NAVIGATION_h

#include <vector>

#include "XRFunction.h"

class XRNavigation {
 public:
  void init();

  XRFunction& currentFunction() const;
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