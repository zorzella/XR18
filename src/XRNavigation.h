#ifndef XR_NAVIGATION_h
#define XR_NAVIGATION_h

#include <vector>

#include "XRFunction.h"

class XRNavigation {
 public:
  void init();

  const XRFunction& currentFunction() const;
  void goRight();
  void goLeft();
  void goUp();
  void goDown();

 private:
  void buildFunctions();
};

#endif