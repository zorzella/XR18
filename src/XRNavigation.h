#ifndef XR_NAVIGATION_h
#define XR_NAVIGATION_h

#include <vector>

#include "XRFunction.h"

class XRNavigation {
 public:
  XRNavigation();

  // TODO: return &XRFunction ?
  const XRFunction currentFunction() const;
  const void goRight() const;

  // const XRFunction right(const XRFunction &other) const;
  // const XRFunction left(const XRFunction &other) const;

 private:
  // const XRFunction m_currentFunction;
  int foo;

  const int rowPosition(const XRFunction& other) const;

  const XRFunction& right(const XRFunction& other) const;
};

#endif