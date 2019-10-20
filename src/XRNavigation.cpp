#include <bits/stdc++.h>
#include <iterator>
#include <utility>
#include <vector>

#include <ArduinoTrace.h>

#include "XRFunction.h"
#include "XRFunctionDescription.h"
#include "XRNavigation.h"
#include "XRRowDescription.h"

const std::vector<XRFunctionDescription> oneToSixteenFuncs(
    const std::string channelNo) {
  return std::vector<XRFunctionDescription>{
      XRFunctionDescription{"Gain", "headamp/" + channelNo + "/gain", 0.5}};
};

int numRows = 2;

const std::vector<XRFunctionDescription> CH01_FUNCTION_DESCRIPTIONS{
    {XRFunctionDescription{"Gain", "headamp/01/gain", 0.5}},
};

const std::vector<XRFunctionDescription> CH02_FUNCTION_DESCRIPTIONS{
    {XRFunctionDescription{"Gain", "headamp/02/gain", 0.5}},
};

const std::vector<XRRowDescription> ROWS{
    XRRowDescription{"CH01", CH01_FUNCTION_DESCRIPTIONS},
    XRRowDescription{"CH02", CH02_FUNCTION_DESCRIPTIONS},
};

std::map<std::pair<int, int>, XRFunction> m_functions;

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

static void buildFunctions(
    std::map<std::pair<int, int>, XRFunction>& allFunctions) {
  std::pair<int, int> pair{0, 0};
  XRFunction value{CH01_FUNCTION_DESCRIPTIONS[0], 0, 0};
  allFunctions.insert({{0, 0}, value});
  // for (int i = 0; i < 1; i++) {
  //   XRRowDescription row = buildRowDescription(i);
  //   for (int j = 0; j < row.funcs().size(); j++) {
  //     auto pair = std::pair<int, int>{i, j};
  //     auto func = XRFunction{row.funcs()[j], i, j};
  //     allFunctions.insert({pair, func});

  //   }
  // }
}

XRFunction* m_currentFunction;

XRNavigation::XRNavigation() {
  m_functions = {};
  buildFunctions(m_functions);
  if (m_functions.find({0, 0}) == m_functions.end()) {
    Serial.printf("No functions found at 0,0. Size is %i\n",
                  m_functions.size());
    return;
  }
  XRFunction f = m_functions.at({0, 0});
  m_currentFunction = &f;
  Serial.printf("Current function name: %s\n",
                m_currentFunction->func().name().c_str());
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
const void XRNavigation::goRight() const {
  XRFunction f = right(*m_currentFunction);
  m_currentFunction = &f;
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
