// #include <map>
#include <bits/stdc++.h>
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

const XRRowDescription CH01{"CH01", oneToSixteenFuncs("01")};
const XRRowDescription CH02{"CH02", oneToSixteenFuncs("02")};

// TODO: delete
const std::vector<XRRowDescription> ROWS{
    CH01,
    CH02,
};

// std::vector<XRFunction> m_functions;
std::map<std::pair<int, int>, XRFunction> m_functions;

static std::map<std::pair<int, int>, XRFunction> buildFunctions() {
  std::map<std::pair<int, int>, XRFunction> result{};
  for (int i = 0; i < 1; i++) {
    XRRowDescription row = ROWS[i];
    for (int j = 0; j < row.funcs().size(); j++) {
      result.emplace(std::pair<int, int>{i, j},
                     XRFunction(row, row.funcs()[j], i, j));
    }
  }

  return result;
}

XRNavigation::XRNavigation() { m_functions = buildFunctions(); }

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

const XRFunction XRNavigation::right(const XRFunction &other) const {
  int rowPos = other.hPos();  // rowPosition(other);
  rowPos++;
  if (rowPos == ROWS.size()) {
    rowPos = 0;
  }

  // other.vPos()

  throw - 1;
  // return ROWS[rowPos];
}

// const XRFunction left(const XRFunction &other) const  {
//       int rowPos = rowPosition(other);
//   rowPos--;
//   if (rowPos == -1) {
//     rowPos = ROWS.size() - 1;
//   }
//   return ROWS[rowPos];
// };
