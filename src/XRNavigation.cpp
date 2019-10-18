#include <vector>

#include "XRFunction.h"

class XRNavigation {
 private:
  std::vector<XRFunction> rows;

  int position(XRFunction that) {
    for (int i = 0; i < rows.size(); i++) {
      if (rows[i].oscAddr() == that.oscAddr()) {
        return i;
      }
    }
    throw - 1;
  }

 public:
  XRFunction right(XRFunction that) {
    int resultPosition = position(that);
    resultPosition++;
    if (resultPosition == rows.size()) {
      resultPosition = 0;
    }
    return rows[resultPosition];
  }

  XRFunction left(XRFunction that) {
        int resultPosition = position(that);
    resultPosition--;
    if (resultPosition == -1) {
      resultPosition = rows.size() - 1;
    }
    return rows[resultPosition];
  };
};