#include <vector>

#include "XRFunction.h"

#include "XRNavigation.h"
#include "XRRowDescription.h"

const XRRowDescription CH01{"CH01"};
const XRRowDescription CH02{"CH02"};

const std::vector<XRRowDescription> rows{
    CH01,
    CH02,
};

const int XRNavigation::rowPosition(const XRFunction &other) const {
  const XRRowDescription otherDesc = other.row();
  const std::string thatRowName = otherDesc.rowName();
  for (int i = 0; i < rows.size(); i++) {
    const XRRowDescription foo = rows[i];
    const std::string fooRowName = foo.rowName();
    if (fooRowName == thatRowName) {
      return i;
    }
  }
  throw - 1;
}

const XRFunction XRNavigation::right(const XRFunction &other) const {
  int resultPosition = rowPosition(other);
  resultPosition++;
  if (resultPosition == rows.size()) {
    resultPosition = 0;
  }

  throw -1;
  // return rows[resultPosition];
}

// const XRFunction left(const XRFunction &other) const  {
//       int resultPosition = rowPosition(other);
//   resultPosition--;
//   if (resultPosition == -1) {
//     resultPosition = rows.size() - 1;
//   }
//   return rows[resultPosition];
// };
