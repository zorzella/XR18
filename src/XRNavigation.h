#ifndef XR_NAVIGATION_h
#define XR_NAVIGATION_h

#include <vector>

#include "XRFunction.h"

class XRNavigation {
  private:
  const int rowPosition(const XRFunction &other) const;

  XRNavigation();
 public:
  const XRFunction right(const XRFunction &other) const;
  // const XRFunction left(const XRFunction &other) const;
};

#endif