// #include <map>
#include <bits/stdc++.h>
#include <iterator>
#include <utility>
#include <vector>

#include "XRFunction.h"
#include "XRFunctionDescription.h"
#include "XRNavigation.h"
#include "XRRowDescription.h"

const std::vector<XRFunctionDescription> oneToSixteenFuncs(
    const std::string channelNo) {
  return std::vector<XRFunctionDescription>{
      XRFunctionDescription{"Gain", "headamp/" + channelNo + "/gain", 0.5}};
};

// const XRRowDescription CH01{"CH01", oneToSixteenFuncs("01")};
// const XRRowDescription CH02{"CH02", oneToSixteenFuncs("02")};

// TODO: delete
// const std::vector<XRRowDescription> ROWS{
//     CH01,
//     CH02,
// };

int numRows = 2;

std::map<const std::pair<int, int>, const XRFunction> m_functions;

static XRRowDescription buildRowDescription(int i) {
  if (i < 0 || i >= numRows) {
    throw - 1;
  }
  if (i < 16) {
    char buffer[10];
    sprintf(buffer, "CH0%d", i);
    std::string name = buffer;
    sprintf(buffer, "0%d", i);
    std::string channelNo = buffer;
    return XRRowDescription{name, oneToSixteenFuncs(channelNo)};
  }
  throw - 1;
}

static std::map<const std::pair<int, int>, const XRFunction> buildFunctions() {
  std::map<const std::pair<int, int>, const XRFunction> result{};
  for (int i = 0; i < 1; i++) {
    XRRowDescription row = buildRowDescription(i);
    for (int j = 0; j < row.funcs().size(); j++) {
      result.emplace(std::pair<int, int>{i, j},
                     XRFunction(row, row.funcs()[j], i, j));
    }
  }

  return result;
}

XRFunction* m_currentFunction;

XRNavigation::XRNavigation() {
  m_functions = buildFunctions();
  XRFunction f = m_functions.at({0, 0});
  m_currentFunction = &f;
}

// const int XRNavigation::rowPosition(const XRFunction &other) const {
//   const XRRowDescription otherDesc = other.row();
//   const std::string thatRowName = otherDesc.rowName();
//   for (int i = 0; i < ROWS.size(); i++) {
//     const XRRowDescription foo = ROWS[i];
//     const std::string fooRowName = foo.rowName();
//     if (fooRowName == thatRowName) {
//       return i;
//     }
//   }
//   throw - 1;
// }

// ch/../config/name

/**
 * @return the XRFunction to go to when the current function is @a other and
 * the "right" button is pressed.
 */
const XRFunction& XRNavigation::right() const {
  return right(*m_currentFunction);
}

const XRFunction& XRNavigation::right(const XRFunction& other) const {
  int rowPos = other.hPos();
  rowPos++;
  if (rowPos == numRows) {
    rowPos = 0;
  }

  const XRFunction& result = m_functions.at({rowPos, other.vPos()});

  return result;
}

const XRFunction& XRNavigation::current() const { return *m_currentFunction; }

// const XRFunction left(const XRFunction &other) const  {
//       int rowPos = rowPosition(other);
//   rowPos--;
//   if (rowPos == -1) {
//     rowPos = ROWS.size() - 1;
//   }
//   return ROWS[rowPos];
// };
