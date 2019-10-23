#ifndef XR_NAVIGATION_h
#define XR_NAVIGATION_h

#include <vector>

#include "XRFunction.h"

class XRNavigation {
 public:
  XRNavigation();

  void init();

  const XRFunction& currentFunction() const;
  void goRight();
  void goLeft();
  void goUp();
  void goDown();

 private:
  // int foo;

  void buildFunctions();

  const int rowPosition(const XRFunction& other) const;

  const XRFunction& right(const XRFunction& other) const;
};

#endif